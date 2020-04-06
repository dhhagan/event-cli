# event-cli
Command Line Interface for logging and evaluating the MIT E-VENT device.

## Virtual Environment

To make development life easier, we recommend working within a virtual environment that is set up to handle all package requirements for QuantAQ. The following instructions assume you have `pyenv` installed. You should also be using python >= 3.5 for all development work to ensure things will go smoothly.


```sh
# create a virtualenv if one does not exist
$ pyenv virtualenv 3.6.5 event

# activate the environment
$ pyenv activate event

# upgrade pip
$ pip install -U pip
```

## Installation

To install the `event-cli`, simply install directly from pip over ssh:

```sh
$ pip install -U git+ssh://git@github.com/dhhagan/event-cli.git@<version-or-branch>

or

$ pip install -U git+https://github.com/dhhagan/event-cli.git@<version-or-branch>
```

*the below section is likely not needed*

### Installing (or upgrading) requirements

Coming soon.

## Environment Variables

None as of now.


## Use

After installation, you should have a native command-line interface program. To use the program, simply 
execute `event-cli <command>` at your command line (while the virtual env is activated!).

```sh
# list all available commands
$ event-cli --help
```

The `--help` command will list all available commands. You can then obtain all other information with the following format:

```sh
$ event-cli <command> --help
```

<hr />

### API Reference



#### Serial Logging

This function logs serial data to file - specifically for encoder validation.

```sh
$ event-cli log [OPTIONS]
```

```sh
Options:
  --port TEXT           The port the Arduino is connected to  [required]
  --baud INTEGER        The baudrate
  --timeout INTEGER     The serial timeout
  --fpath, TEXT         The file path where data will be stored
  --stream, BOOLEAN     Print output to serial?
  --help                Show this message and exit.
```

**Example**

```sh
$ event-cli log --port /dev/cu.usbmodem14441 --fpath "test.csv"
```


#### Unit testing the hardware

This function runs an automated testing protocol and logs all parameters to file.

```sh
$ event-cli test [OPTIONS]
```

```sh
Options:
  --port TEXT           The port the Arduino is connected to  [required]
  --delay INTEGER       The number of seconds each test should last
  --baud INTEGER        The baudrate
  --timeout INTEGER     The serial timeout
  --fpath, TEXT         The file path where data will be stored
  --config, TEXT        Path to the yaml config file with settings to iterate over
  --help                Show this message and exit.
```

**Example**

```sh
$ event-cli test --delay 20 --port /dev/cu.usbmodem14441 --fpath "test-data.csv"
```

**YAML Config File**

If you would like to provide a special configuration file with custom test ranges for BPM, Tidal Volume, and IE Ratio, it should be structured in Yet Another Markup Language (YAML) format. It should have three keys: *ie_ratio*, *beats_per_minute*, and *tidal_volume*. An example files looks like:

**config.yaml**

```yaml
beats_per_minute: [5, 10, 15, 20, 25, 30, 35, 40]
tidal_volume: [100, 200, 300, 400, 500, 600, 700, 800]
ie_ratio: [1, 2, 3, 4]
```

Once the file is created, you can force the test script to use it using the `config` flag as documented above.