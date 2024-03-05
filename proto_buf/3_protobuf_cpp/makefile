default:
	protoc --proto_path=./ --cpp_out=./ addressbook.proto
	# after running the above code addressbook.pb.h and addressbook.pb.cc file will be generated
	g++ main.cpp addressbook.pb.cc -lprotobuf -pthread -o main
	./main