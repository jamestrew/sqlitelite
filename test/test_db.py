from testing import cli


def test_empty_select():
    result = cli.run_cmds(["select"])
    assert result == ["db > Executed."]


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
    for idx in range(1, 1401):
        cmds.append(f"insert {idx} jt jt@gmail.com")
    result = cli.run_cmds(cmds)
    assert result.pop() == "db > Error: Table full."


def test_max_length_string_fields():
    long_username = "a" * 32
    long_email = "a" * 255
    result = cli.run_cmds(
        [
            f"insert 1 {long_username} {long_email}",
            "select",
            ".exit",
        ]
    )
    assert result == [
        "db > Executed.",
        f"db > \t1\t{long_username}\t{long_email}",
        "Executed.",
    ]


def test_negative_id():
    result = cli.run_cmds(["insert -1 jt jt@gmail.com", "select"])
    assert result == [
        "db > ID must be a positive integer.",
        "db > Executed.",
    ]


def test_non_int_id():
    result = cli.run_cmds(["insert foo jt jt@gmail.com", "select"])
    assert result == [
        "db > ID must be a positive integer.",
        "db > Executed.",
    ]


def test_data_persistence():
    cli.run_cmds(["insert 1 jt jt@gmail.com", ".exit"])
    result = cli.run_cmds(["select", ".exit"])

    assert result == [
        "db > \t1\tjt\tjt@gmail.com",
        "Executed.",
    ]


def test_print_constants():
    result = cli.run_cmds([".constants", ".exit"])
    assert result == [
        "db > Constants:",
        "ROW_SIZE: 293",
        "COMMON_NODE_HEADER_SIZE: 6",
        "LEAF_NODE_HEADER_SIZE: 10",
        "LEAF_NODE_CELL_SIZE: 297",
        "LEAF_NODE_SPACE_FOR_CELLS: 4086",
        "LEAF_NODE_MAX_CELLS: 13",
    ]


def test_print_btree_node():
    cmds = []
    for id in [3, 1, 2]:
        cmds.append(f"insert {id} user{id} person{id}@example.com")
    cmds.extend([".btree", ".exit"])

    result = cli.run_cmds(cmds)
    assert result == [
        "db > Executed.",
        "db > Executed.",
        "db > Executed.",
        "db > Tree:",
        "leaf (size 3)",
        " - 0 : 1",
        " - 1 : 2",
        " - 2 : 3",
    ]


def test_duplicate_id_error():
    result = cli.run_cmds(
        [
            "insert 1 user1 user1@email.com",
            "insert 1 user1 user1@email.com",
            "select",
            ".exit",
        ]
    )

    assert result == [
        "db > Executed.",
        "db > Error: Duplicate key.",
        "db > \t1\tuser1\tuser1@email.com",
        "Executed.",
    ]
