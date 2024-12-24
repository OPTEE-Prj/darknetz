**Edit coco.data File:**
```
classes= 80
train  = data/train.txt
valid  = data/valid.txt
names = data/coco.names
backup = backup/
eval=coco
```

**Create Necessary Directory Structure:**
```
mkdir -p data backup
```

```
# Download COCO dataset
wget http://images.cocodataset.org/zips/train2017.zip
wget http://images.cocodataset.org/zips/val2017.zip
wget http://images.cocodataset.org/annotations/annotations_trainval2017.zip
```

```
# Unzip the downloaded files
unzip train2017.zip -d data/
unzip val2017.zip -d data/
unzip annotations_trainval2017.zip -d data/
```

```
# Generate train.txt and valid.txt with the absolute paths of the images
find $(pwd)/data/train2017 -name "*.jpg" > data/train.txt
find $(pwd)/data/val2017 -name "*.jpg" > data/valid.txt
```

```
# Download the coco.names file
wget https://raw.githubusercontent.com/pjreddie/darknet/master/data/coco.names -O data/coco.names

```


```
manjaecho@BOOK-ONJSU68FMQ:~/devel/optee64/share/mjcho/data$ cat valid.txt | head
/home/manjaecho/devel/optee64/share/mjcho/data/val2017/000000488385.jpg
/home/manjaecho/devel/optee64/share/mjcho/data/val2017/000000047585.jpg
/home/manjaecho/devel/optee64/share/mjcho/data/val2017/000000433374.jpg
/home/manjaecho/devel/optee64/share/mjcho/data/val2017/000000491613.jpg
/home/manjaecho/devel/optee64/share/mjcho/data/val2017/000000404923.jpg
/home/manjaecho/devel/optee64/share/mjcho/data/val2017/000000157365.jpg
/home/manjaecho/devel/optee64/share/mjcho/data/val2017/000000377000.jpg
/home/manjaecho/devel/optee64/share/mjcho/data/val2017/000000125405.jpg
/home/manjaecho/devel/optee64/share/mjcho/data/val2017/000000404568.jpg
/home/manjaecho/devel/optee64/share/mjcho/data/val2017/000000087875.jpg
manjaecho@BOOK-ONJSU68FMQ:~/devel/optee64/share/mjcho/data$ sed -i 's|/home/manjaecho/devel/optee64/share/mjcho/data/|/mnt/host/share/mjcho/data/|g' valid.txt
manjaecho@BOOK-ONJSU68FMQ:~/devel/optee64/share/mjcho/data$ cat valid.txt | head
/mnt/host/share/mjcho/data/val2017/000000488385.jpg
/mnt/host/share/mjcho/data/val2017/000000047585.jpg
/mnt/host/share/mjcho/data/val2017/000000433374.jpg
/mnt/host/share/mjcho/data/val2017/000000491613.jpg
/mnt/host/share/mjcho/data/val2017/000000404923.jpg
/mnt/host/share/mjcho/data/val2017/000000157365.jpg
/mnt/host/share/mjcho/data/val2017/000000377000.jpg
/mnt/host/share/mjcho/data/val2017/000000125405.jpg
/mnt/host/share/mjcho/data/val2017/000000404568.jpg
/mnt/host/share/mjcho/data/val2017/000000087875.jpg
```
