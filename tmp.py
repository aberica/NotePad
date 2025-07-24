```
ef make_new_table(_depth: int, _table_name: str, _parent_table: Optional[Table]) -> Table:
    nonlocal  structured_shared, subtitle_stack
    _table_nCol = structured_shared[_table_name].nCol or 1 if _table_name in structured_shared.keys() else 1
    _table_nRow = structured_shared[_table_name].nRow or 1 if _table_name in structured_shared.keys() else 1
    _table_subtitle = " / ".join([e.contents.text for e in subtitle_stack]) if subtitle_stack else ""
    _new_table = Table(
        depth=_depth,
        nCol=_table_nCol,
        nRow=_table_nRow,
        name=_table_name,
        subtitle=_table_subtitle,
        content=[[""]*_table_nCol for _ in range(_table_nRow)],
        childs=[],
        parent=_parent_table
    )
    return _new_table

parent_table = table_root
if cur.clientInfo.parentType == "body" and cur.clientInfo.parentObjectName is None:
    flush_table_block(table)
    table_root = make_new_table(0, 'body', None)
else:
    parent_table = find_parent_table(table_root, cur.clientInfo.parentObjectName)

for c in cur.clientInfo.containObjects:
    new_table = make_new_table(parent_table.depth + 1, c.objectName, parent_table)
    parent_table.childs.append(new_table)
```
는 정상작동해?
