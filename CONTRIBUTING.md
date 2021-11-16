<!-- omit in toc -->
# Contributing to Nebula

First off, thanks for taking the time to contribute! ❤️

All types of contributions are encouraged and valued. See the [Table of Contents](#table-of-contents) for different ways to help and details about how this project handles them. Please make sure to read the relevant section before making your contribution. It will make it a lot easier for us maintainers and smooth out the experience for all involved. The community looks forward to your contributions. 🎉

> And if you like the project, but just don't have time to contribute, that's fine. There are other easy ways to support the project and show your appreciation, which we would also be very happy about:
> - Star the project
> - Tweet about it
> - Refer this project in your project's readme
> - Mention the project at local meetups and tell your friends/colleagues

<!-- omit in toc -->
## Table of Contents

- [I Have a Question](#i-have-a-question)
- [I Want To Contribute](#i-want-to-contribute)
- [Reporting Bugs](#reporting-bugs)
- [Suggesting Enhancements](#suggesting-enhancements)
- [Your First Code Contribution](#your-first-code-contribution)
- [Improving The Documentation](#improving-the-documentation)
- [Styleguides](#styleguides)
- [Commit Messages](#commit-messages)
- [Join The Project Team](#join-the-project-team)

## I Have a Question

> If you want to ask a question, we assume that you have read the available [Documentation](https://jjsheets.github.io/nebula/).

Before you ask a question, it is best to search for existing [Issues](https://github.com/jjsheets/nebula/issues) that might help you. In case you have found a suitable issue and still need clarification, you can write your question in this issue. It is also advisable to search the internet for answers first.

If you then still feel the need to ask a question and need clarification, we recommend the following:

- Open an [Issue](https://github.com/jjsheets/nebula/issues/new).
- Provide as much context as you can about what you're running into.
- Provide project and platform versions (nodejs, npm, etc), depending on what seems relevant.

We will then take care of the issue as soon as possible.

<!--
You might want to create a separate issue tag for questions and include it in this description. People should then tag their issues accordingly.

Depending on how large the project is, you may want to outsource the questioning, e.g. to Stack Overflow or Gitter. You may add additional contact and information possibilities:
- IRC
- Slack
- Gitter
- Stack Overflow tag
- Blog
- FAQ
- Roadmap
- E-Mail List
- Forum
-->

## I Want To Contribute

> ### Legal Notice <!-- omit in toc -->
> When contributing to this project, you must agree that you have authored 100% of the content, that you have the necessary rights to the content and that the content you contribute may be provided under the project license.

### Reporting Bugs

<!-- omit in toc -->
#### Before Submitting a Bug Report

A good bug report shouldn't leave others needing to chase you up for more information. Therefore, we ask you to investigate carefully, collect information and describe the issue in detail in your report. Please complete the following steps in advance to help us fix any potential bug as fast as possible.

- Make sure that you are using the latest version.
- Determine if your bug is really a bug and not an error on your side e.g. using incompatible environment components/versions (Make sure that you have read the [documentation](https://jjsheets.github.io/nebula/). If you are looking for support, you might want to check [this section](#i-have-a-question)).
- To see if other users have experienced (and potentially already solved) the same issue you are having, check if there is not already a bug report existing for your bug or error in the [bug tracker](https://github.com/jjsheets/nebula/issues?q=label%3Abug).
- Also make sure to search the internet (including Stack Overflow) to see if users outside of the GitHub community have discussed the issue.
- Collect information about the bug:
- Stack trace (Traceback), Log Files, Screenshots, or Videos
- OS, Platform and Version (Windows, Linux, macOS, x86, ARM)
- Version of the interpreter, compiler, SDK, runtime environment, package manager, depending on what seems relevant.
- Possibly your input and the output
- Can you reliably reproduce the issue? And can you also reproduce it with older versions?

<!-- omit in toc -->
#### How Do I Submit a Good Bug Report?

> You must never report security related issues, vulnerabilities or bugs to the issue tracker, or elsewhere in public. Instead sensitive bugs must be sent by email to .
<!-- You may add a PGP key to allow the messages to be sent encrypted as well. -->

We use GitHub issues to track bugs and errors. If you run into an issue with the project:

- Open a [Bug Report](https://github.com/jjsheets/nebula/issues/new?assignees=&labels=bug&template=bug_report.md&title=). Do not adjust the default Assignees, Labels, Projects, or Milestones settings.
- Explain the behavior you would expect and the actual behavior.
- Please provide as much context as possible and describe the *reproduction steps* that someone else can follow to recreate the issue on their own. This usually includes your code. For good bug reports you should isolate the problem and create a reduced test case if possible.
- Provide the information you collected in the previous section.

Once it's filed:

- The project team will label the issue accordingly.
- A team member will try to reproduce the issue with your provided steps. If there are no reproduction steps or no obvious way to reproduce the issue, the team will ask you for those steps and mark the issue as `needs-repro`. Bugs with the `needs-repro` tag will not be addressed until they are reproduced.
- If the team is able to reproduce the issue, it will be marked `needs-fix`, as well as possibly other tags (such as `critical`), and the issue will be left to be [implemented by someone](#your-first-code-contribution).

### Suggesting Enhancements

This section guides you through submitting an enhancement suggestion for Nebula, **including completely new features and minor improvements to existing functionality**. Following these guidelines will help maintainers and the community to understand your suggestion and find related suggestions.

<!-- omit in toc -->
#### Before Submitting an Enhancement

- Make sure that you are using the latest version.
- Read the [documentation](https://jjsheets.github.io/nebula/) carefully and find out if the functionality is already covered, maybe by an individual configuration.
- Perform a [search](https://github.com/jjsheets/nebula/issues) to see if the enhancement has already been suggested. If it has, add a comment to the existing issue instead of opening a new one.
- Find out whether your idea fits with the scope and aims of the project. It's up to you to make a strong case to convince the project's developers of the merits of this feature. Keep in mind that we want features that will be useful to the majority of our users and not just a small subset. If you're just targeting a minority of users, consider writing an add-on/plugin library.

<!-- omit in toc -->
#### How Do I Submit a Good Enhancement Suggestion?

Enhancement suggestions are tracked as [Feature requests](https://github.com/jjsheets/nebula/issues/new?assignees=&labels=enhancement&template=feature_request.md&title=).

- Use a **clear and descriptive title** for the issue to identify the suggestion.
- Provide a **step-by-step description of the suggested enhancement** in as many details as possible.
- **Describe the current behavior** and **explain which behavior you expected to see instead** and why. At this point you can also tell which alternatives do not work for you.
- You may want to **include screenshots or videos** which help you demonstrate the steps or point out the part which the suggestion is related to. You can use [this tool](https://www.cockos.com/licecap/) to record GIFs on macOS and Windows, and [this tool](https://github.com/colinkeenan/silentcast) or [this tool](https://github.com/GNOME/byzanz) on Linux.
- **Explain why this enhancement would be useful** to most Nebula users.

<!-- omit in toc -->
### Your First Code Contribution

- To contribute code, you should set up your working environment so that you can successfully use git from the command line, checkout code (both Nebula and submodules), install other dependencies, compile, and test your changes.
- To install git, follow the instructions [here]().
- To checkout code, run the following from a command line: `git clone https://github.com/jjsheets/nebula.git`, `git submodule init`, and `git submodule update`. This will retrieve the source code for Lua, YAML-cpp, GLFW, and SQLite needed for later steps.

<!-- omit in toc -->
#### Dependencies (Linux)

- To install dependencies on Linux (Ubuntu is supported, but any repo which supports apt should work):

```
sudo apt-get update
sudo apt-get install -y tup libvulkan-dev libxi-dev libxxf86vm-dev libreadline-dev xorg-dev lcov
mkdir external/glslc
cd external/glslc
curl -fsS "https://storage.googleapis.com/shaderc/badges/build_link_linux_clang_release.html" | awk -v 'RS=<' -F\> '/^meta http-equiv="refresh"/ && $1 ~ /url=/{split($1,a,"[?=\"]");exit} END{print "url=\""a[7]"\""}' | curl -f --remote-name-all -K-
tar -xf install.tgz
```

- If tup is unavailable via your distribution's package manager, download it [here]().
- Other distributions should have similar methods to install these required development files and libraries, but specifics are not provided.

<!-- omit in toc -->
#### Lua Compilation (Linux)

Nebula uses Lua built as C++ with the following instructions on Linux:

```
cd external/lua
sed -i 's/CC= gcc/CC= g++/g' makefile
make all
cd ../..
```

<!-- omit in toc -->
#### YAML Compilation (Linux)

Nebula uses YAML-cpp compiled normally:

```
cd external/yaml-cpp
cmake .
make yaml-cpp
cd ../..
```

<!-- omit in toc -->
#### SQLite dependency

Nebula uses the SQLite Amalgamation, which can be retrieved with the following commands:

```
cd external
curl https://www.sqlite.org/2021/sqlite-amalgamation-3360000.zip -o sql.zip
7z x -bso0 -bsp0 sql.zip
mv sqlite-amalgamation-3360000 sqlite-build
```

<!-- omit in toc -->
#### GLFW Compilation (Linux)

Nebula needs the following, at minimum, compiled from GLFW:

```
cd external/glfw
cmake -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF .
make glfw
cd ../..
```

<!-- omit in toc -->
#### Nebula compilation

Nebula uses tup as its build system. As a result, building is simple once all dependencies are correctly installed and/or built. Just run the following command in the project root: `tup`.

You can run the test suite after compiling with: `test/test`. You can run the Nebula sample application with: `build/nebula`.

<!-- omit in toc -->
### Improving The Documentation

All documentation for Nebula is held in the `gh-pages` branch in the `/docs` directory. Make changes to files in this branch on github itself and submit pull requests; never commit changes directly.

<!-- omit in toc -->
## Styleguides


<!-- omit in toc -->
### Commit Messages

Commit messages should begin with a single line description of the change made, which is no more than 72 characters. Further information can be provided in later lines after inserting a blank line. The first line of the commit message should be an imperative sentence without final punctuation, such as: `Add implementation for widget transmogrification`.

<!-- omit in toc -->
### Source Code Formatting

If possible, all C++ code should be formatted using Clang-format, using the .clang-format file in the project root directory. This is not necessary, but doing so will streamline the process of getting pull requests accepted. Use .cc file extension for C++ source files, and *.h for C++ header files.

All C++ source files and headers should begin with the following comment:

```
// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details
```

Header files must additionally include a header guard after the license notice comment, such as the following:

```
#ifndef NEBULA_BIND_H
#define NEBULA_BIND_H

... header file contents

#endif // NEBULA_BIND_H
```

Header files in the `/src` directory should follow the above pattern, replacing non-alphanumeric characters with underscores following the `NEBULA_` prefix. Headers in the `/test` directory should use `NEBULA_TEST_` as the prefix.

<!-- omit in toc -->
## Attribution
This guide is based on the **contributing-gen**. [Make your own](https://github.com/bttger/contributing-gen)!
