import click
from pathlib import Path
import os
import sys
import shutil
from datetime import datetime
import serial

@click.group()
def cli():
    pass


@click.command(name="log")
@click.option("--port", default=None, help="The port the Arduino is connected to", type=str, required=True)
@click.option("--baud", default=115200, help="The baudrate", type=int, required=False)
@click.option("--timeout", default=1, help="The serial timeout", type=int, required=False)
@click.option("--fpath", default="data.csv", help="The file path where data will be stored", type=str)
@click.option("--stream", default=True, help="Print output to serial?", type=bool)
def log(port, baud, timeout, fpath, stream):
    """Log Arduino serial data to a file.
    """
    if stream:
        print ("Logging streaming data...")

    # setup the serial connection
    try:
        ser = serial.Serial(port, baud, timeout=timeout)
    except Exception as e:
        sys.exit("Serial connection setup failed")
        print (e)

    # open a file for writing
    with open(fpath, "w") as fobj:
        # write a header
        fobj.write("timestamp_local,millis,enc\n")
        fobj.flush()

        # run a loop and read/write
        while True:
            rv = ser.readline().decode("utf-8")

            if len(rv) > 0:
                rv = rv.strip()

                # add a real timestamp
                rv = str(datetime.now()) + "," + rv + "\n"

                fobj.write(rv)

                if stream:
                    print (rv)




cli.add_command(log)