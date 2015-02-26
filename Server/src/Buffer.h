// =====================================================================================
// 
//       Filename:  Buffer.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年02月21日 20时05分01秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Gaoyuan, sishuiliunian0710@gmail.com
//        Company:  Class 1204 of Computer Science and Technology
// 
// =====================================================================================

#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <sys/uio.h>
#include <errno.h>
#include <string>
#include <boost/implicit_cast.hpp>
#include <iostream>
#include <unistd.h>
using std::string;
using boost::implicit_cast;
using std::vector;

namespace liunian{
class Buffer{
	public:
		static const size_t kPrependSize = 8;
		static const size_t kInitialSize = 1024;

		Buffer(size_t initSize = kInitialSize)
			:buffer(kPrependSize + initSize),
			readIndex(kPrependSize),
			writeIndex(kPrependSize)
		{
			
		}
		void swap (Buffer &data_){
			buffer.swap(data_.buffer);
			std::swap(readIndex, data_.readIndex);
			std::swap(writeIndex, data_.writeIndex);
		}
		size_t readableSize() const {
			return writeIndex - readIndex;
		}
		size_t writeableSize()const{
			return buffer.size() - writeIndex;
		}
		size_t prependSize() const {
			return readIndex;
		}
		const char * readStartIndex() const{
			return begin() + readIndex;
		}
//		const char *findCRLF() const{
//			const char * crlf = std::search(readStartIndex(), writeStartIndex(), kCRLF, kCRLF + 2);
//			return crlf == writeIndex() ? NULL:crlf;
//		}
//		const char *findCRLF(const char *start) const{
//			const char * crlf = std::search(start, writeStartIndex(), kCRLF, kCRLF + 2);
//			return crlf == writeIndex() ? NULL:crlf;
//		}
//		const char* findEOL() const{
//			const void* eol = memchr(readStartIndex(), '\n', readableSize());
//			return static_cast<const char*>(eol);
//		}
//		const char* findEOL(const char* start) const{
//			const void* eol = memchr(start, '\n', writeSartIndex() - start);
//			return static_cast<const char*>(eol);
//		}

		void retrieve(size_t len){
			if (len < readableSize()){
				readIndex += len;
			}
			else{
				retrieveAll();
			}
		}
		void retrieveUntil(const char *flag){
			retrieve(flag - readStartIndex());
		}
		void retrieveAll(){
			readIndex = kPrependSize;
			writeIndex = kPrependSize;
		}
		string retrieveAsString(){
			string str(readStartIndex(), readableSize());
		//	std::cout << "In Buffer " << str << std::endl;
			retrieveAll();
			return str;
		}
		string retrieveAllAsString(){
			return retrieveAllAsString(readableSize());
		}
		string retrieveAllAsString(size_t len){
			string result(readStartIndex(), len);
			retrieve(len);
			return result;
		}
		void append(const string &str){
			append(str.c_str(), str.length());
		}
		void append(const char * data_, size_t len){
			ensureWriteable(len);
			std::copy(data_, data_ + len, writeStartIndex());
			writeIndex += len;
		}
		void append(const void * data_, size_t len){
			append(static_cast<const char *>(data_), len);
		}
		void ensureWriteable(int len){
			if (writeableSize() < len){
				makeSpace(len);
			}
		}
		char *writeStartIndex(){
			return begin() + writeIndex;
		}
		ssize_t readFd(int fd, int *savedError){
			char extraBuf[65536];
			struct iovec vec[2];
			const size_t writeableSize_ = writeableSize();
			vec[0].iov_base = begin() + writeIndex;
			vec[0].iov_len = writeableSize_;
			vec[1].iov_base = extraBuf;
			vec[1].iov_len = sizeof(extraBuf);

		//	const int iovcnt = (writeableSize < sizeof(extraBuf)) ? 2 : 1;


			//std::cout << "Before readv " << writeIndex << std::endl;

//			const ssize_t n = read(fd, extraBuf, sizeof(extraBuf));
			const ssize_t n = readv(fd, vec, 2);
		//	const ssize_t n = readv(fd, vec, iovcnt);

			if (n < 0){
				*savedError = errno;
			}
			else if (implicit_cast<size_t>(n) <= writeableSize_){
				writeIndex += n;

			//	std::cout << "Test " << extraBuf << "  " << n << std::endl;
			//std::cout << "after readv " << writeIndex << std::endl;
			for (int i = writeIndex - n; i < writeIndex; i++){
				std::cout << buffer[i];
			}
			}
			else{
				writeIndex = buffer.size();
				append(extraBuf, n - writeableSize_);
			}
			std::cout << std::endl;
			return n;
		}

	private:
		vector<char> buffer;
		size_t readIndex;
		size_t writeIndex;
		static const char kCRLF[];

		char *begin(){
			return &*buffer.begin();
		}
		const char *begin() const{
			return &*buffer.begin();
		}
		void makeSpace(size_t len){
			if (writeableSize() + prependSize() < len + kPrependSize){
				buffer.resize(writeIndex + len);
			}
			else {
				size_t readable = readableSize();
				std::copy(begin() + readIndex, begin() + writeIndex, begin() + kPrependSize);
				readIndex = kPrependSize;
				writeIndex = readIndex + readable;
			}
		}

};
}
#endif
