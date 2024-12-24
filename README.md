<img src="../../../Insoft-UK/blob/main/assets/logo.svg" width="80" height="80" />

# Prime-C for HP Prime
<img src="https://raw.githubusercontent.com/Insoft-UK/Prime-C/main/assets/Prime-C_Logo.svg" style="width: 128px" />
Prime-C is a high-level general-purpose programming language that adds some elements of C to the PPL programming language.
<br />
<br />
This experimental project is expected to undergo significant evolution as development continues. Its primary aim is to assess the feasibility of creating a hybrid language combining PPL and C-like syntax, allowing developers to write code in a unified environment. The language is designed to compile directly into pure PPL for the HP Prime calculator, while also supporting compilation for platforms like macOS. This enables users to test and debug their code natively before deploying it on an actual HP Prime calculator.
<br />
<br />
# UPDATE
The syntax of **Prime-C** is a cut down of C and C++. It can also be adapted to more closely mimic PPL syntax while still compiling into pure PPL by leveraging macros such as AND and OR.

Not all C/C++ features are supported—most notably, pointers are excluded since PPL does not support them. Type casting is supported simply to facilitate compiling and testing on macOS, but when targeting PPL, such casts are simply ignored during compilation as if they were never written.

The development of Prime-C is now largely complete, with the core design finalized. Moving forward, changes will mainly focus on incremental improvements rather than significant revisions.

>[!WARNING]
Experimental, not intended to be used for actual software development, but can be used instead of P+.

>[!NOTE]
When compiling for macOS, the project will use a C++ library to emulate PPL functions. A Xcode SpriteKit based macOS app is utilized to create a simulator for the HP Prime environment to a certain degree, allowing one to easly test and debug an HP Prime app written in Prime-C to a certain degree.
