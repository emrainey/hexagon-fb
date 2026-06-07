# Hexagon AI Agent Guidelines (AGENTS.md)

Welcome to the collaboration guidelines for porting and developing **Hexagon** (a 3D file system browser) on macOS. This file outlines how the AI agent and the developer (USER) should interact.

---

## 1. Collaboration Model & Philosophy

### Primary Roles
- **AI Agent (Antigravity)**: Responsible for the majority of the code implementation, refactoring, building systems, and debugging.
- **Developer (User)**: Acts as the architect, guide, reviewer, and learner. All major changes must be approved by the developer, and the overall journey is a learning exercise.

### Educational-First Interaction
This project is a learning experience for the developer. To support this:
1. **Explain the "Why" and "How"**: The agent must explain its design choices, architectural changes, and code modifications *before* or *during* implementation.
2. **Contextualize Code Changes**: Avoid dropping large blocks of code without explaining key C++ paradigms, OpenGL/GLUT conventions, or macOS system characteristics being utilized.
3. **Interactive Debugging**: When compilation errors occur, explain the root cause (e.g., header search paths, link libraries, API deprecations) rather than just silently fixing it.

---

## 2. Technical Scope for macOS Porting

Getting Hexagon to function on macOS involves addressing several key areas:
- **Build System (CMake)**: Adjusting `CMakeLists.txt` to find and link the appropriate OpenGL, GLUT, and helper libraries on macOS.
- **OpenGL/GLUT Modernization vs. Compatibility**: macOS has deprecated OpenGL (v4.1 is the maximum core profile supported, and legacy OpenGL is restricted in GLUT/Cocoa contexts). We need to verify if the project uses legacy fixed-function OpenGL or modern shader-based OpenGL, and how GLUT/freeglut or GLFW handles the window context on macOS.
- **System and POSIX Port Layer**: Updating `/source/hexagon/port/posix` system APIs to correctly fetch directory listings and execute commands on macOS.
- **Dependency Management**: Leveraging package managers like Homebrew for tools (e.g. GLFW, GLEW, YAML-CPP) while ensuring CMake can reliably locate them.

---

## 3. Communication Guidelines

- **Concise Summaries**: Keep conversational responses concise. Summarize what changed, why it changed, and how to verify it.
- **Use Artifacts for Plans & Walkthroughs**:
  - **`implementation_plan.md`**: For proposing major architectures or steps.
  - **`task.md`**: For checking off items as we build.
  - **`walkthrough.md`**: For summarizing completed work and demonstrating results.
- **C++ Coding Standards**:
  - Keep comments and docstrings intact.
  - Use standard C++17 library elements where possible (e.g., `std::filesystem`).
  - Keep header files clean and structure logic in implementation files.
  - Ensure symbols, functions, and filenames are formatted as clickable file links in discussion (e.g. `[Model.cpp](file:///Users/emrainey/Source/hexagon-fb/source/hexagon/Model.cpp)`).
