# Clox
Clox is an interpeted high level scripting language run on a virtual machine. All work is based on the book *Crafting Interperters* by Robert Nystrom.
This is a project for my self to learn the fundementals of how compilers and interpreters work with the hope of one day creating my own fully operational language.

### Modifications
There are a few modifications that appear in my version of clox that aren't in the original book. One of these is 2 new byte code operations
```
  OP_CONSTANT_SHORT
  OP_CONSTANT_LONG
```
These allow for more than the original 256 max constant values allowed for in the original implementation. This was part of one of the challenges from Robert in chapter 14 
to expand the functionality of the language for ourselves.
