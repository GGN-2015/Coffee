import os


def getAllFilePath(filePath = "programs"):
    dirList = []
    for root, dirs, files in os.walk(filePath):
        for file in files:  #获取文件路径
            dirList.append(file)
    return dirList


successCnt = 0


def compileFile(fileName):
    global successCnt
    outputFileName = "compiled/" + fileName.replace(".txt", ".asm")
    print("compile fileName = " + fileName + " ------------------------------------------------------------ ")
    ret = os.system("coffee programs/" + fileName + " > " + outputFileName)
    if ret == 0:
        successCnt += 1


if __name__ == "__main__":
    fileList = getAllFilePath()
    fileCnt = len(fileList)
    for file in fileList:
        compileFile(file)
    print("compile finished ---------------------------------------------------------------------- ")
    print("total: %d. success: %d." % (fileCnt, successCnt))
    os.system("pause")