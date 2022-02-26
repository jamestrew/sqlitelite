import subprocess


def pytest_configure():
    print("CLEAN AND BUILD")
    subprocess.run(["make", "clean"])
    subprocess.run(["make", "db"])
