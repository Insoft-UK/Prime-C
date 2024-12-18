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
C syntax with some PPL syntax via macros, tho it can be written to resemble PPL syntax more and still compile into pure PPL by using some of the macros like `and` and `or`.

Not all of C fetures be supported such as pointers as PPL dosn't have pointers.  C casting is supported mainly so one can compile and test on macOS tho when compiled for PPL they simply removed as if were never typed.

>[!WARNING]
Experimental, not to be used for actual software development.

>[!NOTE]
When compiling for macOS, the project will use a C++ library to emulate PPL functions. A Xcode SpriteKit based macOS app is utilized to create a simulator for the HP Prime environment, allowing one to easly test and debug an HP Prime app written in Prime-C to a certain degree.
