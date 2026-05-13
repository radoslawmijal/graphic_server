# 3D Scene Interpreter

This project is a C++ application that interprets a custom scripting language to manipulate 3D objects in a scene. It reads an initial scene configuration from an XML file, processes a command script, and sends object state updates to a separate graphics server for visualization. The interpreter is extensible through a plugin system, allowing new commands to be added as shared libraries.

## Features

*   **Extensible Command Set**: Easily add new commands via a plugin architecture.
*   **Complex Scenarios**: Supports both sequential and parallel execution of commands.
*   **Flexible Configuration**: Scene objects and their properties are defined in an XML file.
*   **C-style Preprocessing**: The command script interpreter supports macros (`#define`) and comments (`//`, `/* */`).
*   **Real-time Visualization**: Communicates with a graphics server to display the scene changes.

## Preview

![Server with running example program](https://private-user-images.githubusercontent.com/160499256/591910912-194bcaf5-8012-4da8-b6b7-3a92205d53de.png?jwt=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3Nzg2ODc2NTgsIm5iZiI6MTc3ODY4NzM1OCwicGF0aCI6Ii8xNjA0OTkyNTYvNTkxOTEwOTEyLTE5NGJjYWY1LTgwMTItNGRhOC1iNmI3LTNhOTIyMDVkNTNkZS5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjYwNTEzJTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI2MDUxM1QxNTQ5MThaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT00NjIxNDVhOTNjNDRmMjVhMzc0NmQ4ZGNiZDQyZjcxNzM2MWE3OTlhMDQ5NjI4N2Q0M2UxZmY5YzY1YmQ3NTNkJlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCZyZXNwb25zZS1jb250ZW50LXR5cGU9aW1hZ2UlMkZwbmcifQ.P6-yajV6qJx98JpF6hTZb0_kyVlbiehXFj6f0P5E6TM)

## Dependencies

To build and run this project, you will need:
*   A C++11 (or newer) compiler (e.g., g++)
*   `make`
*   `xterm` (for the graphics server)
*   **Xerces-C++ Library**: For parsing XML files.

### Dependency Installation (Debian/Ubuntu)

```bash
sudo apt-get update
sudo apt-get install build-essential libxerces-c-dev xterm
```

## Building the Project

1.  Clone the repository.
2.  Navigate to the project's root directory.
3.  Compile the project by running the `make` command:

    ```bash
    make
    ```

This will compile the main interpreter executable (`bin/interpreter`), the plugins (`plugin/*.so`), and place them in the appropriate directories.

## Usage

Before running the interpreter, you need to start the graphics server. The project is configured to work with a server that can be launched separately.

To run the interpreter, use the following command from the project's root directory:

```bash
./bin/interpreter <command_file.cmd> <config_file.xml>
```

*   `command_file.cmd`: Path to the script file containing object manipulation commands.
*   `config_file.xml`: Path to the XML file defining the initial scene objects and plugins to load.

Example:
```bash
./bin/interpreter commands.cmd config.xml
```

The interpreter will connect to the graphics server, which should be listening on `127.0.0.1:6217`.

## Project Structure

### Configuration (`config.xml`)

The XML file sets up the initial state of the scene and loads necessary command plugins.

*   **`<Lib Name="libInterp4Rotate.so"/>`**: Loads a command plugin from the directory specified by the `PLUGIN_DIR` environment variable (defaults to `plugin/`).
*   **`<Cube ... />`**: Defines a cuboid object in the scene. Attributes include:
    *   `Name`: A unique identifier for the object.
    *   `Scale`: The size of the object along X, Y, and Z axes.
    *   `Shift`: An initial translation vector.
    *   `RotXYZ_deg`: Initial rotation in degrees around the X, Y, and Z axes.
    *   `RGB`: The color of the object.

### Command Scripting (`.cmd` files)

The script files contain commands to be executed by the interpreter.

*   **Commands**: `CommandName Param1 Param2 ...`
    *   Example: `Rotate Dron_1 OZ 30 360`
*   **Comments**:
    *   Single-line: `// This is a comment`
    *   Multi-line: `/* This is a block comment */`
*   **Macros**: C-style `#define` macros are supported for constants.
    ```
    #define DRONE_SPEED 50
    Move Dron_1 100 DRONE_SPEED
    ```
*   **Execution Blocks**:
    *   **Sequential**: Commands within `Begin_Sequential_Actions` and `End_Sequential_Actions` are executed one after another.
    *   **Parallel**: Commands within `Begin_Parallel_Actions` and `End_Parallel_Actions` are executed concurrently in separate threads. These blocks can be nested.

## Extending the Interpreter

You can add new commands by creating your own plugins.

1.  **Create a Plugin**: In the `plugin/src/` directory, create a new `.cpp` file for your command (e.g., `Interp4MyCommand.cpp`).
2.  **Implement the Command**:
    *   Your command class should inherit from `AbstractInterp4Command`.
    *   Implement the required virtual methods: `ReadParams`, `ExecCmd`, `PrintCmd`, etc.
3.  **Export Functions**: Export the following C-style functions from your shared library:
    *   `AbstractInterp4Command* CreateCmd(void)`: A factory function that creates an instance of your command interpreter object.
    *   `const char* GetCmdName()`: A function that returns the name of the command as a string.
4.  **Update Makefile**: Add rules to the `Makefile` to compile your new plugin into a `.so` file and place it in the `plugin/` directory.
5.  **Load the Plugin**: In your `config.xml` file, add a `<Lib Name="libMyCommand.so"/>` tag to load your new plugin at runtime.

---

This project was developed as part of a university course on advanced programming techniques. Some files were provided by the course teacher, Dr. Bogdan Kreczmer.