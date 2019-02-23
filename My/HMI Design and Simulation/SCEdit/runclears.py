import os
def debug_info(level,string):
    if level==1:
        print('\033[1;36;40m')
        print(string)
        print('\033[0m')
    if level==2:
        print('\033[1;31;40m')
        print(string)
        print('\033[0m')
def replaceAll(old, new, str):
    str = str.replace(old, new)
    return str
def getfiles(path_being_checking):
    try:
        files = os.listdir(path_being_checking)
        for f in files:
            mypath = os.path.join(path_being_checking, f)
            debug_info(1,mypath)
            if os.path.isfile(mypath):
                (filepath, tempfilename) = os.path.split(mypath)
                if tempfilename=="clear.py":
                    debug_info(1,mypath)
                    os.system('python "'+mypath+'"')
                    debug_info(1,"python "+mypath)
            if os.path.isdir(mypath):
                input("subfolder will be checked:"+mypath)
                os.chdir(mypath)
                getfiles(mypath)#递归
    except  Exception as e :
        debug_info(2,str(e))


getfiles(os.getcwd())
