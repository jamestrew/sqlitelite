from testing import cli


def test_insert_and_select_basic():
    result = cli.run_cmds(["insert 1 jt jt@gmail.com", "select"])
    assert result == [
        "db > Executed.",
        "db > \t1\tjt\tjt@gmail.com",
        "Executed.",
    ]


def test_exit():
    result = cli.run_cmds([".exit"])
    assert not result


def test_table_full_error():
    cmds = []
    for _ in range(1401):
        cmds.append("insert 1 jt jt@gmail.com")
    result = cli.run_cmds(cmds)
    assert result.pop() == "db > Error: Table full."
