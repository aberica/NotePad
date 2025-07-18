import json
import os
from collections import Counter
from dataclasses import dataclass
from typing import List, Optional, Dict, Union
import re       # 승윤 추가


# === 데이터 클래스 정의 ===
@dataclass
class ContainObject:
    objectName: str

@dataclass
class ParaShape:
    charBold: bool
    charFontName: str
    charHeight: int
    paraAlign: str
    paraLeftMargin: int
    paraRightMargin: int
    styleName: Optional[str] = None

@dataclass
class ClientInfo:
    breakFlags: Optional[int] = None
    containObjects: Optional[List[ContainObject]] = None
    paraShape: Optional[ParaShape] = None
    parentType: Optional[str] = None
    parentObjectName: Optional[str] = None
    colAddr: Optional[int] = None
    rowAddr: Optional[int] = None
    colSpan: Optional[int] = None
    rowSpan: Optional[int] = None
    cellFlags: Optional[int] = None

@dataclass
class Contents:
    text: str

@dataclass
class InferenceInfo:
    inferenceType: str
    inferenceLevel: Optional[int] = None

@dataclass
class PosInfo:
    docPageNum: int
    listId: int
    paraId: int

@dataclass
class BodyElement:
    clientInfo: ClientInfo
    contents: Contents
    inferenceInfo: InferenceInfo
    posInfo: PosInfo

# 승윤 추가
@dataclass
class SharedElement:
    caption: Optional[str]
    nCol: int
    nRow: int
    objectName: str
    objectType: str


# === 파싱 함수 ===
def parse_para_shape(raw: dict) -> ParaShape:
    """
    paraShape에 대한 raw dict 데이터를 ParaShape 객체로 변환한다.

    Args:
        raw (dict): JSON에서 추출된 paraShape 데이터

    Returns:
        ParaShape: 변환된 ParaShape 인스턴스
    """
    return ParaShape(
        charBold=raw.get('charBold', False),              # 글자가 bold 처리되었는지 여부 (기본값: False)
        charFontName=raw.get('charFontName', ''),         # 글꼴 이름 (기본값: 빈 문자열)
        charHeight=raw.get('charHeight', 0),              # 글자 높이 (예: 1500 = 15pt, 기본값: 0)
        paraAlign=raw.get('paraAlign', 'LEFT'),           # 문단 정렬 (LEFT, RIGHT, CENTER, JUSTIFY 등)
        paraLeftMargin=raw.get('paraLeftMargin', 0),      # 왼쪽 여백 (기본값: 0)
        paraRightMargin=raw.get('paraRightMargin', 0),    # 오른쪽 여백 (기본값: 0)
        styleName=raw.get('styleName')                    # 스타일 이름 (선택적, 없을 경우 None)
    )

def parse_body_element(data: dict) -> BodyElement:
    """
    단일 JSON 객체(dict)로부터 BodyElement 인스턴스를 생성한다.

    Args:
        data (dict): 원본 JSON 데이터 (문서의 본문 요소 1건)

    Returns:
        BodyElement: 파싱된 BodyElement 객체
    """

    # clientInfo 하위 속성 처리
    client_info_data = data.get('clientInfo', {})

    # containObjects는 리스트 형태이므로 개별 ContainObject 인스턴스로 변환
    contain_objects = [
        ContainObject(**obj) for obj in client_info_data.get('containObjects', [])
    ]

    # paraShape가 존재할 경우 전용 파서로 처리 (예: dict → ParaShape 객체)
    para_shape = (
        parse_para_shape(client_info_data['paraShape'])
        if 'paraShape' in client_info_data
        else None
    )

    client_info = ClientInfo(
        breakFlags=client_info_data.get('breakFlags'),
        containObjects=contain_objects,
        paraShape=para_shape,
        parentType=client_info_data.get('parentType'),
        parentObjectName=client_info_data.get('parentObjectName'),
        colAddr=client_info_data.get('colAddr'),
        rowAddr=client_info_data.get('rowAddr'),
        colSpan=client_info_data.get('colSpan'),
        rowSpan=client_info_data.get('rowSpan'),
        cellFlags=client_info_data.get('cellFlags')
    )

    # contents는 단일 객체이므로 그대로 언패킹 (기본값: {"text": ""})
    contents = Contents(**data.get('contents', {"text": ""}))

    # inferenceInfo는 추론 관련 정보 (기본값: inferenceType만 존재)
    inference_info = InferenceInfo(**data.get('inferenceInfo', {"inferenceType": "none"}))

    # posInfo는 문단 위치 정보 (기본값 지정)
    pos_info = PosInfo(**data.get('posInfo', {"docPageNum": 0, "listId": 0, "paraId": 0}))

    # 최종 BodyElement 인스턴스 반환
    return BodyElement(
        clientInfo=client_info,
        contents=contents,
        inferenceInfo=inference_info,
        posInfo=pos_info
    )

# 승윤추가
def parse_shared_element(data: dict) -> SharedElement:
    """
    단일 JSON 객체(dict)로부터 BodyElement 인스턴스를 생성한다.

    Args:
        data (dict): 원본 JSON 데이터 (문서의 본문 요소 1건)

    Returns:
        SharedElement: 파싱된 SharedElement 객체
    """

    caption = data.get('caption')
    n_col = data.get('nCol', 0)
    n_row = data.get('nRow', 0)
    object_name = data.get('objectName', '')
    object_type = data.get('objectType', '')

    return SharedElement(
        caption=caption,
        nCol=n_col,
        nRow=n_row,
        objectName=object_name,
        objectType=object_type
    )

# 승윤추가
def fix_index(
        structured_body: List[BodyElement],
        structured_shared: Dict[str, SharedElement],
    ) -> List[BodyElement]:

    passing_count = 0
    delete_indices = []
    for i, cur in enumerate(structured_body):
        if passing_count != 0:
            passing_count -= 1
            delete_indices.append(i)
            continue

        parent_type = cur.clientInfo.parentType
        is_table_inner = (
                parent_type in ("table", "textbox") or
                (parent_type == "body" and cur.clientInfo.parentObjectName)
        )
        if not is_table_inner:
            continue
        if cur.clientInfo.rowAddr != 1 or cur.clientInfo.colAddr != 1:
            continue

        table_name = cur.clientInfo.parentObjectName
        n_row = structured_shared[table_name].nRow
        n_col = structured_shared[table_name].nCol
        if n_row != 1:
            continue
        if cur.contents.text.startswith(tuple(map(str, range(1, 10)))):
            continue

        for j in range(1, n_col):
            cur.contents.text += ' ' + structured_body[i+j].contents.text
        passing_count = n_col - 1

    for idx in sorted(delete_indices, reverse=True):
        del structured_body[idx]

    return structured_body


# 승윤 추가
def is_subtitle(cur: BodyElement, nxt: BodyElement, markers: Dict[str, int], subtitle_threshold=15) -> bool:
    if len(cur.contents.text) > subtitle_threshold:
        return False

    def check_str_marker(s):
        s = s.strip()
        if re.match(r'^제\d+장', s):
            return 'jeNjang'
        if re.match(r'^제\d+조', s):
            return 'jeNjo'
        if re.match(r'^\d+\.', s):
            return 'num'
        if re.match(r'^[가-힣]\.', s):
            return 'hanguel'
        if s and '\u2460' <= s[0] <= '\u2473':
            return 'circlenum'
        return 'No'  # 해당 없음

    # 현재 보고 있는 Body와 그 다음 Body의 위계값. 값이 높을수록 높은 위계
    cur_rank = nxt_rank = 0

    # 가운데 정렬 vs 그 외 정렬
    if cur.clientInfo.paraShape and cur.clientInfo.paraShape.paraAlign == 'JUSTIFY':
        cur_rank += 10000
    if nxt.clientInfo.paraShape and nxt.clientInfo.paraShape.paraAlign == 'JUSTIFY':
        nxt_rank += 10000

    # 목차부호
    cur_marker = check_str_marker(cur.contents.text)
    nxt_marker = check_str_marker(nxt.contents.text)
    if cur_marker not in markers.keys():
        markers[cur_marker] = len(markers)
    cur_rank += 9000 - markers[cur_marker] * 1000
    nxt_rank += 9000 - (markers[nxt_marker] * 1000 if nxt_marker in markers.keys() else 9000)
    if nxt_marker in markers.keys():
        for key in list(markers.keys()):
            if markers[key] > markers[nxt_marker]:
                del markers[key]

    if cur.clientInfo.paraShape is not None and nxt.clientInfo.paraShape is not None:
        # 글자크기
        if cur.clientInfo.paraShape.charHeight > nxt.clientInfo.paraShape.charHeight:
            cur_rank += 100
        elif cur.clientInfo.paraShape.charHeight < nxt.clientInfo.paraShape.charHeight:
            nxt_rank += 100

        # bold체
        if cur.clientInfo.paraShape.charBold:
            cur_rank += 10
        if nxt.clientInfo.paraShape.charBold:
            nxt_rank += 10

        # 좌측여백
        if cur.clientInfo.paraShape.paraLeftMargin > nxt.clientInfo.paraShape.paraLeftMargin:
            cur_rank += 1
        elif cur.clientInfo.paraShape.paraLeftMargin< nxt.clientInfo.paraShape.paraLeftMargin:
            nxt_rank += 1

    return cur_rank > nxt_rank

# 승윤 변경
def build_final_json_blocks(
        structured_body: List[BodyElement],
        structured_shared: Dict[str, SharedElement],
    ) -> List[Dict]:
    """
    구조화된 BodyElement 목록과 인덱싱된 헤더 요소를 기반으로
    최종 JSON 블록(text, table 등)을 구성하는 함수.

    - parentType이 table/textbox/footnote인 경우 특수 처리
    - 나머지는 일반 텍스트 처리
    - stack을 사용해 subtitle을 동적으로 구성
    """

    # subtitle 생성을 위한 stack (헤더 역할)
    stack: List[BodyElement] = []
    results: List[Dict] = []

    # 현재 처리 중인 블록 관련 상태 변수들
    current_block_type = None  # "text" 또는 "table"
    current_subtitle = ""      # 현재 subtitle
    current_table_key = None   # 테이블 고유 키
    current_table = [[""]]     # 테이블 내용
    markers: Dict[str, int] = {}

    def flush_table_block():
        nonlocal current_block_type, current_subtitle, current_table_key, current_table
        if current_table == [[""]]:
            return

        # results에 추가
        table_lines = []
        for row in range(structured_shared[current_table_key].nRow):
            line = "^B".join(current_table[row])
            table_lines.append(line)
        results.append({
            "type": "table",
            "anchor": "",
            "page": "",
            "subtitle": current_subtitle,
            "content": table_lines
        })

        # 상태 초기화
        current_block_type = None
        current_table_key = None
        current_table = [[""]]

    for prev, cur, nxt in zip(structured_body[:1] + structured_body[:-1], structured_body, structured_body[1:] + structured_body[-1:]):
        text = cur.contents.text.strip()
        pos_key = (cur.posInfo.docPageNum, cur.posInfo.listId, cur.posInfo.paraId)
        parent_type = cur.clientInfo.parentType
        is_table_outer = (
                cur.clientInfo.containObjects is not None and
                cur.clientInfo.containObjects[0].objectName.startswith('t')
        )
        is_table_inner = (
                parent_type in ("table", "textbox") or
                (parent_type == "body" and cur.clientInfo.parentObjectName)
        )

        if is_table_outer:
            table_name = cur.clientInfo.containObjects[0].objectName
            current_table = [[""]*structured_shared[table_name].nCol for _ in range(structured_shared[table_name].nRow)]

        if text == "":
            continue  # 빈 텍스트는 무시

        bool_subtitle = is_subtitle(cur, nxt, markers)
        if bool_subtitle:
            stack.append(cur)

        if is_table_inner:
            table_key = cur.clientInfo.parentObjectName or f"{parent_type}-{pos_key}"
            row_addr = cur.clientInfo.rowAddr or 0  # 기본값 0
            row_span = cur.clientInfo.rowSpan or 1
            col_addr = cur.clientInfo.colAddr or 0
            col_span = cur.clientInfo.colSpan or 1

            # 새로운 테이블 시작 시 이전 테이블 flush
            if current_block_type != "table" or table_key != current_table_key:
                flush_table_block()
                current_block_type = "table"
                current_table_key = table_key
                current_subtitle = " / ".join([e.contents.text.strip() for e in stack]) if stack else ""

            # 행별 cell 저장
            for i in range(row_span):
                current_table[row_addr + i][col_addr] = text
            for i in range(col_span):
                current_table[row_addr][col_addr + i] = text
        elif not bool_subtitle:
            continuing_content = False
            if len(results) > 0 and prev.clientInfo.paraShape and cur.clientInfo.paraShape:
                if (prev.clientInfo.paraShape.charHeight == cur.clientInfo.paraShape.charHeight and
                    prev.clientInfo.paraShape.charBold == cur.clientInfo.paraShape.charBold and
                    prev.clientInfo.paraShape.paraLeftMargin == cur.clientInfo.paraShape.paraLeftMargin):
                    results[-1]["content"] += cur.contents.text
                    continuing_content = True

            if not continuing_content:
                subtitle = " / ".join([e.contents.text.strip() for e in stack]) if stack else ""
                results.append({
                    "type": "text",
                    "anchor": "",
                    "page": "",
                    "subtitle": subtitle,
                    "content": text
                })

    return results



# === 실행부 ===
if __name__ == "__main__":
    current_directory = os.getcwd()
    # 상위 폴더로 이동

    parent_directory = os.path.dirname(current_directory)
    BASE_PATH = os.path.join(parent_directory, 'file\\')  # 기본 경로 설정
    input_path = BASE_PATH + "3. 2022년도 국정감사 결과보고서.json"

    file_base_name = os.path.splitext(os.path.basename(input_path))[0]

    with open(input_path, 'r', encoding='utf-8') as f:
        json_data = json.load(f)

    structured_body: List[BodyElement] = [parse_body_element(item) for item in json_data['body']]
    structured_shared: Dict[str, SharedElement] = {item.get('objectName', ''): parse_shared_element(item) for item in json_data['sharedData']}
    structured_body = fix_index(structured_body)

    final_json = build_final_json_blocks(structured_body, structured_shared)

    output_dir = "output"
    os.makedirs(output_dir, exist_ok=True)

    # 4. 출력 파일 경로 설정
    output_file = os.path.join(output_dir, f"converted_{file_base_name}.json")

    with open(output_file, "w", encoding="utf-8") as f:
        json.dump(final_json, f, ensure_ascii=False, indent=2)

    # 완료 메시지 출력
    print(f"변환 완료 → {output_file}")
