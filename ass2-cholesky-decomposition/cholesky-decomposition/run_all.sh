filenames=(
  "chol1"
  "chol2"
  "chol3"
  "chol4"
  "chol5"
  "chol6"
  "chol7"
)

for filename in "${filenames[@]}"; do
  make run FILE="$filename"
done
