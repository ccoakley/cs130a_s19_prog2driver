#!/bin/bash

current_dir=${pwd}
student_executables_dir=/cs/class/cs130a/TURNIN/prog2/
test_dir=/cs/class/cs130a/test/prog2/
test_input_dir=/cs/class/cs130a/prog2_tests/
test_output_dir=/cs/class/cs130a/prog2_outputs/
prog2_grades=/cs/class/cs130a/prog2_grades.txt
grader_output=/cs/class/cs130a/grader_output_2.txt

cd ${student_executables_dir}
student_files=$(ls | grep -v LOGFILE)
cd ${current_dir}

# make sure test dir exists (create if it doesn't)
if [ ! -d ${test_dir} ]; then
  mkdir -p ${test_dir}
fi

# make sure the input and ouput directories exist, die if they don't
for important in ${test_input_dir} ${test_output_dir}; do
  if [ ! -d ${important} ]; then
    echo ${important} must exist with appropriate files
    exit 1
  fi
done

cd ${test_input_dir}
input_files=$(ls *.txt)
cd ${current_dir}

# initialize gradefile
echo initializing the grade files
echo '' > ${prog2_grades}
echo '' > ${grader_output}

for student_file in ${student_files}; do
  trimmed=$(echo $student_file | sed -e 's/-/./' | cut -d'.' -f1)
  latest=$(ls -c ${student_executables_dir}${trimmed}-*.tar.Z | head -n1)
  latest=$(echo $latest | rev | cut -d'/' -f1 | rev)
  if [[ -z $latest ]]; then
    latest=${student_file}
  fi
  if [[ ! ${student_file} = ${latest} ]]; then
    echo skipping ${student_file} because ${latest} >> ${grader_output}
  else
    cd ${current_dir}
    echo ${student_file} >> ${prog2_grades}
    echo ${student_file} >> ${grader_output}
    echo Testing ${student_file}
    date
    stat -c %y "${student_executables_dir}${student_file}" >> ${prog2_grades}
    rm -rf ${test_dir}
    mkdir -p ${test_dir}
    cp "${student_executables_dir}${student_file}" ${test_dir}
    cd ${test_dir}
    tar -xzvf ${student_file}
    # alternative for absolute path
    # find . -iname makefile | xargs readlink -f
    codedir=$(dirname "$(readlink -f "$(find . -iname "Makefile" | head -n1)")") || echo Makefile not found >> ${prog2_grades}
    echo codedir: ${codedir}
    cd ${codedir}
    make
    ls "${codedir}"
    if [[ -f ./prog2 ]]; then
      prog=prog2
    else
      echo "no prog2 found, trying to be smarter" >> ${prog2_grades}
      if [[ $(ls -F | grep '*' | cut -d'*' -f1 | wc -w) -eq 1 ]]; then
        echo "found a single executable" >> ${prog2_grades}
        prog=$(ls -F | grep '*' | cut -d'*' -f1)
      else
        echo "unknown main executable" >> ${prog2_grades}
      fi
    fi

    if [[ -f ./${prog} ]]; then
      for input_file in ${input_files}; do
        index=$(echo $input_file | cut -d'_' -f1)
        echo $index
        cat ${test_input_dir}${input_file} | ./${prog} | sed -e "s/[ \t]*$//" > out_${input_file}
        diff out_${input_file} ${test_output_dir}out_${input_file} | head -n10 >> ${grader_output}
        if [[ 0 -eq $(diff out_${input_file} ${test_output_dir}out_${input_file} | wc -l) ]]; then
          echo "${index}: yes" >> ${prog2_grades}
        else
          echo "${index}: no" >> ${prog2_grades}
        fi
      done
    else
      echo "continuing to next student; no main executable" >> ${prog2_grades}
    fi
    echo '' >> ${prog2_grades}
    cd ${current_dir}
    rm -rf ${test_dir}
  fi
done
