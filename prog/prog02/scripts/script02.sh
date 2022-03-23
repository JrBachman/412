target=$1
path=$2
txtarray=()
num_levels=0
num_files=0
num_dirs=0
function traverse() {
    num_levels=$(($num_levels+1))
    for file_search in "$path"/*
    do
        path=$file_search
        if [ -d "$file_search" ]; then
            num_dirs=$(($num_dirs+1))
            traverse $file_search
        
        elif [ -f "$file_search" ]; then
            num_files=$(($num_files+1))
            txtarray+=($file_search)
        fi
    done

}
traverse $path
echo "Recursive search for target $target starting at $2"
echo "Search depth: $num_levels"
echo "Total number of folders: $num_dirs" 
echo "Total number of text files: $num_files" 


gcc ../programs/prog02.c -o prog02
./prog02 $target "${txtarray[@]}"