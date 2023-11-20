#ifndef __PARSE_H__
#define __PARSE_H__

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

using Message_ID = uint32_t;        // 32
using Service_ID = uint16_t;        // 16
using Method_ID = uint16_t;         // 15
using Client_ID = uint16_t;         // 16
using Session_ID = uint16_t;        // 16
using Event_ID = uint16_t;          // 15
using Length = uint32_t;            // 32
using Request_ID = uint32_t;        // 32
using Protocol_Version = uint32_t;  // 8
using Interface_Version = uint32_t; // 8
using Message_Type = uint32_t;      // 8
using Return_Code = uint32_t;       // 8
using Payload = string;             // variable size

struct Header {
  Message_ID messageId;
  Service_ID serId;
  Method_ID methodId;
  Client_ID clientId;
  Session_ID sessId;
  Event_ID eventId;
  Length len;
  Request_ID reqsId;
  Protocol_Version protoV;
  Interface_Version interfaceV;
  Message_Type messageType;
  Return_Code rtCode;
} Header;

class SOMEIP_parse {
public:
  SOMEIP_parse(string path) {
    filePath = path;
    generateCode();
    header.reserve(5);
  }

  void getHeader(int messageNumber);
  void printPayload(int messageNumber);
  void printHeader(int messageNumber);
  void writeOutfile();
  vector<struct Header> header;

private:
  void generateCode();
  string content;
  ifstream infile;
  bool eventOrmethod;
  string filePath;
};

#endif