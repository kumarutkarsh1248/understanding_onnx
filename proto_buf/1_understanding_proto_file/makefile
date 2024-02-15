default:
	protoc -I=./ --cpp_out=./ person.proto
	g++ main.cpp person.pb.cc -lprotobuf -pthread -o main
	./main