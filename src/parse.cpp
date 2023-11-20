#include "../include/parse.hpp"
#include <gtest/gtest.h>

void SOMEIP_parse::generateCode() {
  ifstream file(filePath);
  string str;
  while (file >> str) {
    content += str;
  }
}

void SOMEIP_parse::getHeader(int messageNumber) {
  int offset = 2 * (messageNumber == 0
                        ? 0
                        : header[messageNumber - 1].len +
                              8); // one Byte convert to two numbers base 16.
  header[messageNumber].serId =
      stoi(content.substr(offset + 0, 4), nullptr, 16);
  eventOrmethod = (content.substr(offset + 4, 4), nullptr, 16) >= 0x8000;
  if (eventOrmethod) {
    header[messageNumber].eventId =
        stoi(content.substr(offset + 4, 4), nullptr, 16) & 0x3FFF;
  } else {
    header[messageNumber].methodId =
        stoi(content.substr(offset + 4, 4), nullptr, 16) & 0x3FFF;
  }
  header[messageNumber].len = stoi(content.substr(offset + 8, 8), nullptr, 16);
  header[messageNumber].clientId =
      stoi(content.substr(offset + 16, 4), nullptr, 16);
  header[messageNumber].sessId =
      stoi(content.substr(offset + 20, 4), nullptr, 16);
  header[messageNumber].protoV =
      stoi(content.substr(offset + 24, 2), nullptr, 16);
  header[messageNumber].interfaceV =
      stoi(content.substr(offset + 26, 2), nullptr, 16);
  header[messageNumber].messageType =
      stoi(content.substr(offset + 28, 2), nullptr, 16);
  header[messageNumber].rtCode =
      stoi(content.substr(offset + 30, 2), nullptr, 16);
}

void SOMEIP_parse::printHeader(int messageNumber) {
  cout << "// message_" << messageNumber << endl
       << "Service_ID: " << header[messageNumber].serId << endl;
  if (eventOrmethod) {
    cout << "Event_ID: " << header[messageNumber].eventId << endl;
  } else {
    cout << "Method_ID: " << header[messageNumber].methodId << endl;
  }
  cout << "Length: " << header[messageNumber].len << endl
       << "Client_ID: " << header[messageNumber].clientId << endl
       << "Session_ID: " << header[messageNumber].sessId << endl
       << "Protocol_Version: " << header[messageNumber].protoV << endl
       << "Interface_Version: " << header[messageNumber].interfaceV << endl
       << "Message_Type: " << header[messageNumber].messageType << endl
       << "Return_Code: " << header[messageNumber].rtCode << endl;
}

void SOMEIP_parse::printPayload(int messageNumber) {
  int offset = 2 * (messageNumber == 0
                        ? 0
                        : header[messageNumber - 1].len +
                              8); // one Byte convert to two numbers base 16.
  if (offset == 0) {
    cout << endl
         << "SessionID: " << stoi(content.substr(32, 8), nullptr, 16) << endl;
    cout << "// lengthOfArray" << stoi(content.substr(40, 8), nullptr, 16)
         << endl
         << "uniqueID_0: " << stoul(content.substr(48, 8), nullptr, 16) << endl
         << "version_0: " << stoi(content.substr(56, 2), nullptr, 16) << endl;
    cout << "uniqueID_1: " << stoul(content.substr(58, 8), nullptr, 16) << endl
         << "version_1: " << stoi(content.substr(66, 2), nullptr, 16) << endl;
  } else {
    cout << "listEntryExecuted_changedEntry_index: "
         << stoul(content.substr(offset + 32, 8), nullptr, 16) << endl
         << "listEntryExecuted_changedEntry_type: "
         << stoi(content.substr(offset + 40, 2), nullptr, 16) << endl;
  }
  cout << endl << endl;
}

void SOMEIP_parse::writeOutfile() {
  ofstream outfile("answer.txt");
  streambuf *pOld = cout.rdbuf(outfile.rdbuf());
  printHeader(0);
  printPayload(0);
  printHeader(1);
  printPayload(1);
  cout.rdbuf(pOld);
  // cout << get_current_dir_name() << endl;
}

TEST(MockFileStorageTest, testClearJob) {
  string filePath = "./trace.asc";
  SOMEIP_parse myParse(filePath);
  myParse.getHeader(0);
  myParse.getHeader(1);
  myParse.writeOutfile();

  ifstream file1("answer.txt");
  ifstream file2("correct.txt");

  string content1, content2;
  ASSERT_TRUE(file1.is_open());
  ASSERT_TRUE(file2.is_open());

  while (getline(file1, content1)) {
    getline(file2, content2);
    EXPECT_EQ(content1, content2);
  }
}