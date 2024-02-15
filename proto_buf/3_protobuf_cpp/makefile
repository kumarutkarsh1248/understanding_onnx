default:
	protoc --proto_path=./ --cpp_out=./ addressbook.proto
	g++ main.cpp addressbook.pb.cc -lprotobuf -pthread -o main
	./main