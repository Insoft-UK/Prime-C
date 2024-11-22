<img src="../../../Insoft-UK/blob/main/assets/logo.svg" width="80" height="80" />

# Prime C for HP Prime
<img src="https://raw.githubusercontent.com/Insoft-UK/Prime-C/main/assets/Prime-C_Logo.svg" style="width: 128px" />
Prime-C is a compiler for a hybrid PPL + C language that translates code into pure PPL (Prime Programming Language) for the HP Prime calculator.

This experimental project is expected to evolve significantly as development progresses. Its goal is to explore the feasibility of creating a hybrid PPL + C-like language, enabling code to be written in this unified language. The language is designed to compile into pure PPL for the HP Prime calculator while also supporting macOS compilation from the same codebase.

>[!WARNING]
Experimental, not to be used for actual software development.

>[!NOTE]
When compiling for macOS, Windows, or Linux, the project will use a C++ library to emulate PPL functions. SDL will be utilized to create a simulator for the HP Prime environment, allowing the same codebase to run on these platforms.
