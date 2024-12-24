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
