import os

    #if nrootdir exists, clear all the file in the folder
print("目录为: %s" %os.listdir(os.getcwd()))


def delfiles(foldername):
    current_path=os.getcwd()
    target_folder=os.path.join(current_path,foldername)
    try:
        files = os.listdir(target_folder)
        for f in files:
            mypath = os.path.join(target_folder, f)
            print(mypath)
            if os.path.isfile(mypath):
                print("deleting "+mypath)
                os.remove(mypath)
            if os.path.isdir(mypath):
                # print('heihei'+mypath)
                #input("press enter to continue")
                os.chdir(mypath)
                delfiles(mypath)#递归
    except  Exception as e :
        print(str(e))
    os.chdir(current_path)
    os.removedirs(target_folder)


delfiles("tmp")

