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
@click.option("--headers", default="Timestamp,Millis,State,Mode,Pos,Vol,BPM,IE,tIn,tHoldIn,tEx,tHoldOut,vIn,vEx,TrigSens,Pressure", help="Headers for the log file", type=str)
def log(port, baud, timeout, fpath, stream, headers):
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
        # write the header
        fobj.write("t{}\n".format(headers))
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


@click.command("test")
@click.option("--port", default=None, help="The port the Arduino is connected to", type=str, required=False)
@click.option("--delay", default=20, type=int, help="The number of seconds each test should last")
@click.option("--baud", default=115200, help="The baudrate", type=int, required=False)
@click.option("--timeout", default=1, help="The serial timeout", type=int, required=False)
@click.option("--fpath", default="test-steps.csv", help="The file path where data will be stored", type=str)
def test(delay, port, baud, timeout, fpath):
    """Run unittests on the hardware
    """
    import itertools
    from datetime import datetime
    import serial
    import time

    # ie_options = [1, 2, 3, 4]
    # tv_options = [100, 200, 300, 400, 500, 600, 700, 800]
    # bpm_options = [10, 15, 20, 25, 30, 35, 40]

    ie_options = [1, 4]
    tv_options = [250, 750, 1325]
    bpm_options = [10, 40]

    options = list(itertools.product(*[ie_options, tv_options, bpm_options]))

    # how long will this test take?
    total_time_min = len(options)*delay / 60.

    print ("This test will take {} min".format(total_time_min))
    print ("Starting now...")

    # set up a file to write to
    # setup the serial connection
    try:
        ser = serial.Serial(port, baud, timeout=timeout)
    except Exception as e:
        sys.exit("Serial connection setup failed")
        print (e)

    # pause for a second
    time.sleep(1)

    # open a file for writing
    with open(fpath, "w") as fobj:
        # write a header
        fobj.write("timestamp_local,step_no,ie_ratio,tidal_vol,bpm\n")
        fobj.flush()

        for i, (ie_ratio, tidal_vol, bpm) in enumerate(options):
            print ("Step {}/{}\tIE = {:.2f}\tTV = {:.2f}\tBPM = {:.0f}".format(i+1, len(options), ie_ratio, tidal_vol, bpm))

            # write the step to file
            fobj.write("{},{},{:.2f},{:.0f},{:.1f}\n".format(str(datetime.now()), i+1, ie_ratio, tidal_vol, bpm))

            tidal_vol_cmd = "v{:d}\n".format(tidal_vol)
            ie_ratio_cmd = "e{:d}\n".format(ie_ratio)
            bpm_cmd = "b{:d}\n".format(bpm)

            # write the step to the arduino
            ser.write(tidal_vol_cmd.encode("utf-8"))
            ser.write(ie_ratio_cmd.encode("utf-8"))
            ser.write(bpm_cmd.encode("utf-8"))

            # delay
            time.sleep(delay)




cli.add_command(log)
cli.add_command(test)