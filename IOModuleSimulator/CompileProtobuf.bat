@ECHO OFF
ECHO Compiling IOModule.proto...
..\packages\Google.Protobuf.Tools.3.21.12\tools\windows_x64\protoc.exe --csharp_out=. --proto_path=..\IOModule\src IOModule.proto