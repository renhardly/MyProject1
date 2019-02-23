import numpy as np
import cv2
import os
from matplotlib import pyplot as plt
import sys, getopt
from xml.dom.minidom import parse
import xml.dom.minidom
def output_xml(input):
    #在内存中创建一个空的文档
    doc = xml.dom.minidom.Document() 
    #创建一个根节点Managers对象
    root = doc.createElement('Managers') 
    #设置根节点的属性
    root.setAttribute('company', 'xx科技') 
    root.setAttribute('address', '科技软件园') 
    #将根节点添加到文档对象中
    doc.appendChild(root) 

    managerList = [{'name' : 'joy',  'age' : 27, 'sex' : '女'},
                    {'name' : 'tom', 'age' : 30, 'sex' : '男'},
                    {'name' : 'ruby', 'age' : 29, 'sex' : '女'}
                    ]

    for i in managerList :
        nodeManager = doc.createElement('Manager')
        nodeName = doc.createElement('name')
        #给叶子节点name设置一个文本节点，用于显示文本内容
        nodeName.appendChild(doc.createTextNode(str(i['name'])))

        nodeAge = doc.createElement("age")
        nodeAge.appendChild(doc.createTextNode(str(i["age"])))

        nodeSex = doc.createElement("sex")
        nodeSex.appendChild(doc.createTextNode(str(i["sex"])))

    #将各叶子节点添加到父节点Manager中，
    #最后将Manager添加到根节点Managers中
        nodeManager.appendChild(nodeName)
        nodeManager.appendChild(nodeAge)
        nodeManager.appendChild(nodeSex)
        root.appendChild(nodeManager)
    #开始写xml文档
    fp = open(input.rsplit('.',1)[0]+".xml", 'w')
    doc.writexml(fp, indent='\t', addindent='\t', newl='\n', encoding="utf-8")






def parse_xml(input):
    # 使用minidom解析器打开 XML 文档

    DOMTree = xml.dom.minidom.parse(input)
    collection = DOMTree.documentElement
    if collection.hasAttribute("shelf"):
        print("Root element : %s" % collection.getAttribute("shelf"))

    # 在集合中获取所有电影
    movies = collection.getElementsByTagName("movie")

    # 打印每部电影的详细信息
    for movie in movies:
        print("*****Movie*****")
        if movie.hasAttribute("title"):
            print("Title: %s" % movie.getAttribute("title"))

        type = movie.getElementsByTagName('type')[0]
        print("Type: %s" % type.childNodes[0].data)
        format = movie.getElementsByTagName('format')[0]
        print("Format: %s" % format.childNodes[0].data)
        rating = movie.getElementsByTagName('rating')[0]
        print("Rating: %s" % rating.childNodes[0].data)
        description = movie.getElementsByTagName('description')[0]
        print("Description: %s" % description.childNodes[0].data)

def draw_rectangle(inputfile):
    im = cv2.imread(inputfile)
    imgray = cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)
    ret,thresh = cv2.threshold(imgray,127,255,0)
    blursize=101
    while 1:
        blurred_image = cv2.GaussianBlur(thresh,(blursize,blursize),0)
        contours, hierarchy = cv2.findContours(blurred_image,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
        print('contours,blursize "',len(contours),blursize)
        if len(contours)>30:
            break
        if blursize<blurred_image.shape[0]/100:
            break
        if blursize<5:
            break
        blursize=blursize-2
    #if nrootdir exists, clear all the file in the folder
    nrootdir=("./Cutimages/")
    if os.path.isdir(nrootdir):
        dirs = os.listdir("./Cutimages/" )
        for file in dirs:
            if not os.path.isdir(file):
                os.remove("./Cutimages/"+file)
    else:
        os.makedirs(nrootdir)
    #draw all the countours' bounding rectangles and save this images in the temp folder.
    for i in range(0,len(contours)): 
        x, y, w, h = cv2.boundingRect(contours[i])
        cv2.rectangle(blurred_image, (x,y), (x+w,y+h), (153,153,0), 5)
        newimage=im[y+2:y+h-2,x+2:x+w-2] # 先用y确定高，再用x确定宽
        edge_of_subimage = cv2.Canny(newimage,100,200)         
        cv2.imwrite( nrootdir+str(i)+".jpg",edge_of_subimage)
        draw_contour(edge_of_subimage)
        cv2.imwrite( nrootdir+str(i)+"_.jpg",edge_of_subimage)
    #show intrested images
    plt.subplot(121),plt.imshow(im)
    plt.title('Original Image'), plt.xticks([]), plt.yticks([])
    plt.subplot(122),plt.imshow(blurred_image)
    plt.title('Edge Image'), plt.xticks([]), plt.yticks([])
    plt.show()

def draw_contour(im):
    imgray = im
    ret,thresh = cv2.threshold(imgray,127,255,0)
    blurred_image = cv2.GaussianBlur(thresh,(3,3),0)
    contours, hierarchy = cv2.findContours(blurred_image,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    for i in range(0,len(contours)):  
        cv2.drawContours(im, [contours[i]], -1, (0, 255, 0), 2)
    
def main(argv):
    inputfile = ''
    outputfile = ''
    try:
        opts, args = getopt.getopt(argv,"hi:o:",["ifile=","ofile="])
    except getopt.GetoptError:
        print('test.py -i <inputfile> -o <outputfile>')
        sys.exit(2)
    for opt, arg in opts:
      if opt == '-h':
        print('test.py -i <inputfile> -o <outputfile>')
        sys.exit()
      elif opt in ("-i", "--ifile"):
        inputfile = arg
      elif opt in ("-o", "--ofile"):
        outputfile = arg

    print('Input file is "', inputfile)
    print('Output file is "', outputfile)
    
    draw_rectangle(inputfile)
    parse_xml(outputfile)
    output_xml(inputfile)

    
if __name__ == "__main__":
   main(sys.argv[1:])
