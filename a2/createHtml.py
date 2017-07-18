#!/usr/bin/python

import os, sys

def readFormatInfo(filename, tagList, replacementList):
    infoList = []
    try:
        with open(filename + ".info", "r+") as instream:
            for line in instream:
                line = line.replace("\n","")
                infoList.append(line)
        instream.close()
        for member in infoList:
            for tag in tagList:
                if (member[0:len(tag)].upper() == tag):
                    replacementList.append([member[len(tag)+1:], "<" + tag + ">" + member[len(tag)+1:] + "</" + tag + ">"])
        print "readFormatInfo completed\n"
    except IOError:
		print "Cannot open " + filename + ".info\n"    
    return;
    
def formatHtmlText(filename, rawText, replacementList): 
    bodyText = ""
    line = rawText
    for member in replacementList:
        findText = member[0]
        replaceText = member[1]
        line = line.replace(findText, replaceText)
    bodyText = bodyText + line
        
    text = "<Html>"+"<Head><Title>"+filename+"</Title></Head>"+"<Body>"+bodyText+"</Body>"+"</Html>"
    print "formatHtmlText completed\n"
    return text;
    
def generateHtml(filename, rawText):
    tagList = ["B", "I", "U", "H1", "EM"]
    replaceList = []
    
    readFormatInfo(filename, tagList, replaceList)
    htmlText= formatHtmlText(filename, rawText, replaceList)
    print "generateHtml completed\n"
    return htmlText;
   
# open a fifo queue for reading, read 10 strings, close fifo
# reads text from another queue sent by the reader program

pipename = "./q2"
pipe = open(pipename, "r")
filename = pipe.readline()[:-1]
rawText = pipe.readline()[:-1]
os.remove(pipename)
print "Received text from C executable\n"

formattedText = generateHtml(filename, rawText)

pipename = "./q1"
pipe = os.open(pipename, os.O_WRONLY)
os.write(pipe, str(len(formattedText)))
for i in range(len(formattedText)):
    os.write(pipe, formattedText[i])
os.close(pipe)
print "Sent formatted text to C executable\n"


