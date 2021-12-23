@ECHO OFF
ECHO Compiling IOModule.proto...
Protobuf\protoc.exe --nanopb_out=.\src --proto_path=.\src IOModule.proto
COPY src\IOModule.pb.* ..\Pandemic\
