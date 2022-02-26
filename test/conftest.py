import subprocess
import pytest


@pytest.fixture(autouse=True)
def build_and_run():
    subprocess.run(["make", "clean"])
    subprocess.run(["make", "db"])
