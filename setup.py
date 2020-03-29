#!/usr/bin/env python

# -*- coding: utf-8 -*-
"""Setup script for the event CLI"""


from setuptools import setup
from setuptools.command.install import install
import os, shutil
import versioneer


if __name__ == "__main__":
    setup(
        name="event",
        version=versioneer.get_version(),
        cmdclass=versioneer.get_cmdclass(),
        packages=[
            "event", 
        ],
        author='MIT E-VENT Development Team',
        author_email='e-vent@mit.edu',
        url='https://e-vent.mit.edu',
        license=None,
        python_requires=">=3.5",
        entry_points={
            "console_scripts": [
                "event-cli = event.cli:cli"
            ]
        },
    )