#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

#define AFFIRM extern "C" _declspec(dllexport)

AFFIRM SOCKET Connect_Server(char *Smtp_Address,int Port);
AFFIRM void Uninstall(SOCKET Email_Connect);
AFFIRM int Base64_Encode(const unsigned char *sourcedata,char *base64);
AFFIRM void Receive_Message(SOCKET Email_Connect);
AFFIRM void Send_Message(SOCKET Eamil_Connect,char *Message);
AFFIRM void Sun(const char Send_Email[BUFSIZ], const char Send_Passworld[BUFSIZ],char Receive_Email[BUFSIZ],char Title[BUFSIZ],char Content[BUFSIZ]);

const char Base64_Coding[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";


