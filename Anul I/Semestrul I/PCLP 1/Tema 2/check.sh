#!/bin/bash

EXEC="tema2"

function init {
	total_score=0
	pad=$(printf '%.1s' "."{1..70})
	padlength=71

	make
	if [ $? -ne 0 ]; then
		echo "Makefile failed!"
		exit 1
	fi

	mkdir -p output/Task1
	mkdir -p output/Task2/caesar
	mkdir -p output/Task2/vigenere
	mkdir -p output/Task2/addition
	mkdir -p output/Task3/
}

function print_result {
	printf "%s" "$1"
	printf "%*.*s" 0 $((padlength - ${#1} - ${#2} )) "$pad"
	printf "%s\n" "$2"
}

function check_task {
	task_id=$1
	start_test_id=$2
	end_test_id=$3
	test_score=$4
	echo "..............................TASK $task_id..................................."

	for test_id in $(seq $start_test_id $end_test_id); do
		test_file="./input/Task${task_id}/input${test_id}.txt"
            ref_file="./ref/Task${task_id}/ref${test_id}.txt"
            output_file="./output/Task${task_id}/output${test_id}.txt"
	
		./$EXEC < "$test_file" > "$output_file"
	
		if [ -f "$output_file" ]; then
			if diff -w "$output_file" "$ref_file" &> /dev/null; then
				let "total_score += $test_score"
				print_result "Test ${test_id}" "${test_score}/${test_score}p passed"
			else 
				print_result "Test ${test_id}" "0/${test_score}p failed"
			fi
		fi
		rm -f "$output_file"
	done

    echo " "
}

function check_task2 {
    echo "..............................TASK 2..................................."

    for subtask in "caesar" "vigenere" "addition"; do
        start_test_id=0
        end_test_id=4
        
        if [ $subtask == "vigenere" ]; then
            test_score=2
        else
            test_score=4
        fi

        if [ $subtask == "caesar" ]; then
            echo "..............................CAESAR..................................."
        elif [ $subtask == "vigenere" ]; then
            echo ".............................VIGENERE.................................."
        elif [ $subtask == "addition" ]; then
            echo ".............................ADDITION.................................."
        fi

        for test_id in $(seq $start_test_id $end_test_id); do
            test_file="./input/Task2/${subtask}/input${test_id}.txt"
            ref_file="./ref/Task2/${subtask}/ref${test_id}.txt"
            output_file="./output/Task2/${subtask}/output${test_id}.txt"
        
            ./$EXEC < "$test_file" > "$output_file"
        
            if [ -f "$output_file" ]; then
                if diff -w "$output_file" "$ref_file" &> /dev/null; then
                    let "total_score += $test_score"
                    print_result "Test ${test_id}" "${test_score}/${test_score}p passed"
                else 
                    print_result "Test ${test_id}" "0/${test_score}p failed"
                fi
            fi
        done
    done

    echo " "
}

function check_readme {
	echo "..............................TASK README.............................."
    
	ls . | grep -i "readme" &>/dev/null
	if [ $? -eq 0 ]; then
		print_result "Test README" "10/10p passed"
		let "total_score += 10"
	else
		print_result "Test README" "0/10p failed"
	fi
    echo " "
}

function check_style {
	is_style_ok=1
	echo -e "\nRun cpplint\n" > result
	cpplint --filter=-legal/copyright Task1.c Task2.c Task3.c tema2.c &>> result
if [ ! $? -eq 0 ]
then
    is_style_ok=0
fi
	echo -e "\nRun clang-tidy-12\n" >> result
	clang-tidy-12  -checks='-*,cppcoreguidelines*' Task1.c Task2.c Task3.c tema2.c -- &>> result
if [ ! $? -eq 0 ]
then
    is_style_ok=0
fi

if [ $is_style_ok -eq 0 ]
then
    print_result "Test Coding Style" "-10/0p failed"
    let "total_score -= 10"
else
    print_result "Test Coding Style" "0/0p passed"
fi
	# Display run summary
	awk -f parse.awk result

	# Display detailed report
	echo -e "\n==================="
	echo -e "= Detailed report ="
	echo -e "===================\n"
	cat result
	echo " "
	echo " "
}


init
check_task 1 0 4 8 # task1, test0-4, score per test = 8 - 40 pct
check_task2 # task2, 50 pct + 20p bonus inmultire
check_task 3 0 4 10 # task3, test0-4, score per test = 8 - 50 pct
check_readme # 10 pct
check_style

print_result "Final score" "${total_score}/150"
make clean &> /dev/null
