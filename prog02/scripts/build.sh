path=$1
for filename in "$path"/*; do
    gcc $filename -o executable
done