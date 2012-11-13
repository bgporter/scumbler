## Hello!

## Building

Before you can build, assuming that you want to be able to host VST plug-ins, you'll need to sign up as part of the VST developer program with Steinberg and obtain a copy of the VST SDK. Once you've installed that, you'll need to tell your compiler/IDE where to look for the header files.

### OS X / XCode

In XCode, bring up the project settings, and add a new Header Search Path for both the debug and release targets that points at the location of the VST SDK header files, relative to the Builds/MacOSX/ directory on your system. Obviously, this will depend on where you have installed the VST STK; on my system, the correct path entry here is

 `../../../../vst2.x/vstsdk2.4/`
 
 
  
    
### Windows / VS 2010

**TODO: !!!**
