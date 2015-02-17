make clean
make

DATA_PATH=./td
INDEX_FILE=index.dat
NEW_INDEX_FILE=new_index.dat
output="START DATE: `date +"%a_%b_%d_%T_%Y"`\n"
output=${output}"User: $USER OS: `uname`\n\n"

output=${output}"Test argument verification: bad directory\n"
./indexer blah $INDEX_FILE
output=${output}"Result: Error: Directory does not exist\n\n"

output=${output}"Test argument verification: different resulting files\n"
./indexer $DATA_PATH $INDEX_FILE asdf $NEW_INDEX_FILE
output=${output}"Result: Error: resulting file names do not match\n\n"

output=${output}"Test argument verification: incorrect amount of arguments\n"
./indexer $DATA_PATH $INDEX_FILE NEW_INDEX_FILE
output=${output}"Result: Error: insufficient arguments\n\n"

output=${output}"Test with correct parameters\n\n"
output=${output}"Initializing InvertedIndex...\n"
./indexer $DATA_PATH $INDEX_FILE
output=${output}"Index saved in $INDEX_FILE\n\n"

output=${output}"Reinitializing InvertedIndex...\n"
./indexer $DATA_PATH $INDEX_FILE $INDEX_FILE $NEW_INDEX_FILE
output=${output}"New Index saved in $NEW_INDEX_FILE\n\n"


output=${output}"Sorting both files...\n"
sort -d -o $INDEX_FILE $INDEX_FILE
sort -d -o $NEW_INDEX_FILE $NEW_INDEX_FILE

output=${output}"Test equality of files: "
diff -a $INDEX_FILE $NEW_INDEX_FILE

if [ $? -eq 0 ]
then
    output=${output}"Index storage passed test!\n\n"
else
    output=${output}"Index storage failed\n\n"
fi

output=${output}"END DATE: `date +"%a_%b_%d_%T_%Y"`\n\n"
echo -e $output > indexerTestlog.`date +"%a_%b_%d_%T_%Y"`

make clean