# MRA-prototype

MRA (Robocup MSL Reference Architecture) prototype.

After the MSL workshop 2022 at Eindhoven (material reference needed!), Jurge (from VDL RobotSports team) took initiative to drive this forward.

This repository is intended to move from concept idea to demo. Connect with existing Falcons- and RobotSports code bases, facilitate the first demonstration(s) and trigger detailed discussion on how we should proceed.

See here for the [presentation (pdf)](https://drive.google.com/file/d/1-4txLCdpiMcM90y2xGZRdFGJl2T32eTw/view?usp=sharing) from May 2023.

# Demonstration

A demonstration was given at the [Robocup Worldcup 2023 Bordeaux](https://www.robocup.org/events/57), MSL Technical Challenge. It was awarded with **first prize** ([link](https://www.robocup.org/leagues/6)).

It is unclear if there is video footage.

The demo showed how a robot behaves very differently when using Falcons getball+velocitycontrol versus RobotSports getball+velocitycontrol, all via this repository.

[Slides TechChallenge](https://drive.google.com/file/d/14PoBWKEjPzDFVzXhQVmezd2EIjWnwXRg/view?usp=drive_link)

[Slides S/E Challenge](https://docs.google.com/presentation/d/172d7rAMGgPWsQBSFL9lv62un4zYSysuA/edit?usp=sharing&ouid=100208042967383691350&rtpof=true&sd=true)

# Integration

Currently, two build systems are supported: bazel and cmake.

Support for cmake is however a bit ad-hoc, handcrafted. **Help from a cmake wizard would be appreciated!**

For more details and practical guidance, see [INTEGRATION.md](INTEGRATION.md)

# Roadmap

* One or more MRA repositories could appear in the Robocup-MSL github project.
* Releasing and dependency management
* Connect with FMI/FMU standard?
* ...

