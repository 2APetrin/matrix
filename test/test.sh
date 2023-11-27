obj="build/src/matrix"

TEST=".dat"
ANSWR=".ans"

SMALL_names=("00" "01" "02")
BIG_names=("00" "01" "02" "03" "04" "05" "06" "07" "08")

SMALL_folder="test/small_tests/"
BIG_folder="test/big_tests/"

ANS="ans/"
CORRECT="correct/"

echo "SMALL TESTS:"
for i in ${!SMALL_names[@]}; do
    echo
    ${obj} < ${SMALL_folder}${SMALL_names[$i]}${TEST} > ${SMALL_folder}${ANS}${SMALL_names[$i]}${ANSWR}
    diff ${SMALL_folder}${ANS}${SMALL_names[$i]}${ANSWR} ${SMALL_folder}${CORRECT}${SMALL_names[$i]}${ANSWR}
done

echo
echo
echo "BIG TESTS:"
for i in ${!BIG_names[@]}; do
    echo
    ${obj} < ${BIG_folder}${BIG_names[$i]}${TEST} > ${BIG_folder}${ANS}${BIG_names[$i]}${ANSWR}
    diff ${BIG_folder}${ANS}${BIG_names[$i]}${ANSWR} ${BIG_folder}${CORRECT}${BIG_names[$i]}${ANSWR}
done
