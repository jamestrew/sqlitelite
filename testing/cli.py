import subprocess


class DbException(Exception):
    pass


def execute_cmd(db: subprocess.Popen, cmd: str):
    if db.stdin:
        print(cmd, file=db.stdin)


def run_cmds(cmds: list[str]) -> list[str]:
    db = subprocess.Popen(
        ["./db", "mydb.db"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        bufsize=1,
        universal_newlines=True,
    )

    for cmd in cmds:
        execute_cmd(db, cmd)

    try:
        stdout, stderr = db.communicate(timeout=3)
    except TimeoutError as err:
        raise err
    finally:
        db.terminate()

    if stderr:
        raise DbException(stderr)
    return stdout.splitlines()[:-1]
