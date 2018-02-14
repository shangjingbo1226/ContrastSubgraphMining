
import sys
reload(sys)
sys.setdefaultencoding('utf-8')
from PIL import Image

import glob
import numpy as np
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
import seaborn as sn
import pandas as pd

prefix = "output/"
fileList = glob.glob(prefix + "*.out")

print "Generating Image"
for fi in fileList :
    with open(fi, "r") as f :
        graph = f.readline()
        if graph[0:4] == "con " : 
            graph = graph[4:]
            graph = (' '.join((graph[:-1]).split())).split(',')
            gA = graph[0]
            gB = graph[1]
            terms = (f.readline()[:-2]).split(',')
            terms = [ ' '.join(i.split()) for i in terms]
            authors = (f.readline()[:-2]).split(',')
            authors = [' '.join(i.split()) for i in authors]
            n1 = len(authors)
            n2 = len(terms)
            mat1 = {}
            mat2 = {}
            minV = 65536.0
            maxV = -1.0
            for i in range(n1):
                l = f.readline()
                l = l[:-2]
                l = l.split(',')
                l = [float(it) for it in l]
                for j, a in enumerate(l) :
                    mat1[(i,j)] = a
                    if a < minV : minV = a
                    if a > maxV : maxV = a
            for i in range(n1):
                l = f.readline()
                l = l[:-2]
                l = l.split(',')
                l = [float(it) for it in l]
                for j, a in enumerate(l) :
                    mat2[(i,j)] = a 
                    if a < minV : minV = a
                    if a > maxV : maxV = a
            magR = [[0.0, i] for i in range(n1)]
            magC = [[0.0, i] for i in range(n2)]
            for i in range(n1):
                for j in range(n2):
                    magR[i][0] += mat1[(i,j)]
                    magC[j][0] += mat1[(i,j)]
            magR = sorted(magR, reverse = True)
            magC = sorted(magC, reverse = True)
            tempM1 = [[0.0 for i in range(n2)] for j in range(n1)]
            tempM2 = [[0.0 for i in range(n2)] for j in range(n1)]
            for i,it in enumerate(magR):
                for j,jt in enumerate(magC):
                    tempM1[i][j] = mat1[(it[1], jt[1])]
                    tempM2[i][j] = mat2[(it[1], jt[1])]
            mat1 = tempM1
            mat2 = tempM2
    
            newAuthors = ["" for i in range(n1)]
            newTerms = ["" for i in range(n2)]
            for i,it in enumerate(magR):
                newAuthors[i] = authors[it[1]]
            for i,it in enumerate(magC):
                newTerms[i] = terms[it[1]]
            authors = newAuthors
            terms = newTerms
            df_cm = pd.DataFrame(mat1, index = authors, columns = terms)

            plt.figure(figsize = (24,20))
            sn.heatmap(df_cm, annot=False, vmin = minV, vmax = maxV, cmap="Greens")
            plt.title(gA, fontsize = 40)
            plt.xticks(rotation = 90, fontsize = 10)
            plt.yticks(rotation = 0, fontsize = 18)
            plt.savefig(fi + "_1.png") 
            plt.clf()
            plt.close()
    
            df_cm = pd.DataFrame(mat2, index = authors, columns = terms)
            plt.figure(figsize = (24,20))
            sn.heatmap(df_cm, annot=False, vmin = minV, vmax = maxV, cmap="Greens")
            plt.title(gB, fontsize = 40)
            plt.xticks(rotation = 90, fontsize = 10)
            plt.yticks(rotation = 0, fontsize = 18)
            plt.savefig(fi + "_2.png") 
            plt.clf()
            plt.close()
    
            
            images = map(Image.open, [fi+ "_1.png", fi + "_2.png"])
            w,h = zip(*(i.size for i in images))

            t_w = w[0] + int(3.8 / 5.0 * w[0])
            t_h = max(h)

            new_im = Image.new('RGB', (t_w, t_h))

            x_off = 0
            for im in images:
                new_im.paste(im, (x_off, 0))
                x_off += int(3.8 / 5.0 * im.size[0])

            new_im.save(fi + "_bi.png")
            
        else :
            graph = (' '.join((graph[:-1]).split())).split(',')
            gA = graph[0]
            gB = graph[1]
            num = int(f.readline())
            if num == 0 :
                continue
            seeds = set((f.readline()[6:-2]).split(','))
            authors = (f.readline()[:-2]).split(',')
            table = [auth in seeds for auth in authors]
            authors = [' '.join(i.split()) for i in authors]
            mat1 = {}
            mat2 = {}
            minV = 65536.0
            maxV = -1.0
            for i in range(num):
                l = f.readline()
                l = l[:-2]
                l = l.split(',')
                l = [float(it) for it in l]
                for j, a in enumerate(l) :
                    mat1[(i,j)] = a
                    if a < minV : minV = a
                    if a > maxV : maxV = a
            for i in range(num):
                l = f.readline()
                l = l[:-2]
                l = l.split(',')
                l = [float(it) for it in l]
                for j, a in enumerate(l) :
                    mat2[(i,j)] = a
                    if a < minV : minV = a
                    if a > maxV : maxV = a
            magR = [[0.0, 0.0, i] for i in range(num)]
            magC = [[0.0, 0.0, i] for i in range(num)]
            for i in range(num):
                for j in range(num):
                    magR[i][0] += mat1[(i,j)]
                    magC[j][0] += mat1[(i,j)]
            for i in range(num):
                for j in range(num):
                    magR[i][1] += mat2[(i,j)]
                    magC[j][1] += mat2[(i,j)]
            magR = sorted(magR, reverse = True, key = lambda t : t[1])
            magC = sorted(magC, reverse = True, key = lambda t : t[1])
            magR = sorted(magR, reverse = True, key = lambda t : t[0])
            magC = sorted(magC, reverse = True, key = lambda t : t[0])
            magR = sorted(magR, key = lambda t : table[t[2]])
            magC = sorted(magC, key = lambda t : table[t[2]])
            tempM1 = [[0.0 for i in range(num)] for j in range(num)]
            tempM2 = [[0.0 for i in range(num)] for j in range(num)]
            for i,it in enumerate(magR):
                for j, jt in enumerate(magC):
                    tempM1[i][j] = mat1[(it[2], jt[2])]
                    tempM2[i][j] = mat2[(it[2], jt[2])]
            mat1 = tempM1
            mat2 = tempM2

    
            newAuthors = ["" for i in range(num)]

            for i, it in enumerate(magR):
                newAuthors[i] = authors[it[2]]
            authors = newAuthors
            empty = ["" for auth in authors]

            df_cm = pd.DataFrame(mat1, index = authors, columns = authors)
            plt.figure(figsize = (24,20))
            sn.heatmap(df_cm, annot=False, vmin = minV, vmax = maxV, cmap="Greens")
            plt.title(gA, fontsize = 40)
            plt.xticks(rotation = 90)
            plt.yticks(rotation = 0)
            plt.savefig(fi + "_1.png") 
            plt.clf()
            plt.close()
    
            df_cm = pd.DataFrame(mat2, index = empty, columns = authors)
            plt.figure(figsize = (24,20))
            sn.heatmap(df_cm, annot=False, vmin = minV, vmax = maxV, cmap="Greens")
            plt.title(gB, fontsize = 40)
            plt.xticks(rotation = 90)
            plt.yticks(rotation = 0)
            plt.savefig(fi + "_2.png") 
            plt.clf()
            plt.close()
    

            images = map(Image.open, [fi+ "_1.png", fi + "_2.png"])
            w,h = zip(*(i.size for i in images))

            t_w = w[0] + int(2.70 / 5.0 * w[0])
            t_h = max(h)

            new_im = Image.new('RGB', (t_w, t_h))

            images[1] = images[1].crop((int(0.50 / 5.0 * w[0]), 0, w[0], h[0])) 

            x_off = 0
            for im in images:
                new_im.paste(im, (x_off, 0))
                x_off += int(3.80 / 5.0 * im.size[0])

            new_im.save(fi + "_bi.png")


            


