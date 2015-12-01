# TiltIt
Arduino code for a controller connected to a Stewart Platform from MarginallyClever (http://www.marginallyclever.com/product/rotary-stewart-platform-v2/) (https://github.com/MarginallyClever/RotaryStewartPlatform2).

Designs for the controller are available in the controller folder. Part files are .ipt and the assembly is .iam, made with Autodesk Inventor.

See a video of the device in action: https://youtu.be/LdpKwXdAqzQ

Controller design and code made by:
Haukur Hlíðberg, Kristófer Reykjalín Þorláksson & Stefán Óli Valdimarsson.

Stewart Platform design and firmware running on the RUMBA motor controller made by:
Dan Royer, MarginallyClever

----------------------------------------
Notes on the controller:

Some of the part files have icelandic names, thus translations for some parts have been added to the translations.txt file in the controller folder.

The controller has a VGA plug, you can see that in the designs. However you can't use a regular VGA cable to plug the controller to an Arduino since 4 of the 15 pins available in a regular VGA cable are connected together. We had a custom cable made for us with 15 individual wires and soldered the cable to a VGA plug so it'd be easy to connect the controller.
----------------------------------------