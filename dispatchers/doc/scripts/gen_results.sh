#!/bin/bash

[ "$GENERATE_SUBDIAGRAMS" ] || GENERATE_SUBDIAGRAMS=1
[ "$WITH_TITLE" ] || WITH_TITLE=1
DIR=`cd \`dirname $0\`/.. && pwd`
RESULTS=$1
TEST_ASPECTS=$2
IFS=$'\n'
REVISION=`git log --abbrev-commit $DIR/../frameworks | head -1 | awk '{print $2}'`

gen_result()
{
    echo "\begin{landscape}"

    FSMS_COUNT=`ls -tr "$RESULTS/$1" | wc -l`
    echo "\begin{table}"
    echo "\caption{\"$HOSTNAME\" [$REVISION], `echo $1 | sed "s/\_/ /g"`}"
    echo "\centering"
    echo "\begin{longtable}{| c | `for((i=0; i < $FSMS_COUNT; i++)); do echo -n c \|; done`}"
    echo "\hline"

    for fsm in `ls -tr "$RESULTS/$1"`; do
        echo -n "& `echo $fsm | sed \"s/_/\\\\\\_/g\"`";
    done

    echo "\\\\";
    echo "\hline"

    i=1;

    IFS=$' '
    for aspect in $TEST_ASPECTS; do
        echo -n $aspect

        IFS=$'\n'
        for fsm in `ls -tr "$RESULTS/$1"`; do
            cat "$RESULTS/$1/$fsm" | head -$i | tail -1 | xargs -i% echo -n " & %"
        done
        IFS=$' '

        echo "\\\\";
        echo "\hline"

        ((i++))
    done

    echo "\multicolumn{$((FSMS_COUNT+1))}{|c|}{\includegraphics[scale=0.8]{images/$2_all.png}}\\\\"
    echo "\hline"

    echo "\end{longtable}"
    echo "\end{table}"

    echo "\end{landscape}"

    if [ $GENERATE_SUBDIAGRAMS -eq 1 ]; then
        echo "\newpage"
        echo "\begin{table}"
        echo "\centering"
        echo "\begin{longtable}{| c | c |}"
        echo "\hline"

        tmp=0
        for aspect in $TEST_ASPECTS; do
            if [ $tmp -eq 0 ]; then
                echo -n "\includegraphics[scale=0.8]{images/$2_$aspect.png}"
                tmp=1
            else
                echo "& \includegraphics[scale=0.8]{images/$2_$aspect.png}\\\\"
                echo "\hline"
                tmp=0
            fi
        done
        echo "& \\\\ \hline"
        echo "\end{longtable}"
        echo "\end{table}"
    fi

    IFS=$'\n'
}

gen_test()
{
    IFS=$'\n'
    for test in `ls -tr "$RESULTS/$1"`; do
        gen_result "$1/$test" "\"results/$HOSTNAME\"/\"$1\"/$test"
    done
}

gen_tex()
{
    DATE=`date`

    echo "\clearpage"

    echo "\subsection{Results from \"$HOSTNAME\" [$REVISION], ``generated $DATE}"

    echo "\begin{verbatim}"
    echo
    echo "Test aspects:"
    echo

    IFS=$' '
    for aspect in $TEST_ASPECTS; do
        echo "    $aspect:"

        while read line; do
            echo "        $line"
        done < $DIR/aspects/$aspect
        echo
    done

    echo "    test name:"
    echo "        test_NAME[_NUMBER], where NAME is test case name and NUMBER is count of event calls during the test"

    echo "\end{verbatim}"

    echo "\begin{center}"
    echo "\line(1,0){750}"
    echo "\end{center}"

    echo "\begin{verbatim}"
    echo
    echo "Environment statistics:"
    echo
    echo "    generated: $DATE"
    echo "    code revision: $REVISION"
    echo "    hostname: \"$HOSTNAME\""
    echo "    operating system: " `uname -o`
    echo "    processor:" `uname -p`
    echo "    free memory:" `free -m | grep "Mem:" | awk '{print $4}' | xargs -i% echo %Mb`
    echo "    load average: `cat /proc/loadavg`"
    echo "\end{verbatim}"

    echo "\begin{center}"
    echo "\line(1,0){750}"
    echo "\end{center}"

    echo "\begin{verbatim}"
    echo
    echo "All tests summary:"
    echo
	cat $RESULTS.time | xargs -i% echo -e "    %"

    echo "\end{verbatim}"

    echo "\begin{center}"
    echo "\line(1,0){750}"
    echo "\end{center}"

    echo "\begin{verbatim}"
    echo "Results are presented by using table and types of charts:"
    echo

    echo "    table: contains results for each tested aspect and framework"
    echo "    first type of chart: presents relative (0-100%) differences between individual framework and aspect"

    if [ $GENERATE_SUBDIAGRAMS -eq 1 ]; then
        echo "    second type of chart: presents each aspect individually using exact values returned during the test"
    fi

    echo "\end{verbatim}"
    echo "\begin{center}"
    echo "\line(1,0){750}"
    echo "\end{center}"

    IFS=$'\n'
    for test in `ls -tr "$RESULTS"`; do
        gen_test "$test"
    done
}

gen_results()
{
    cd $1 && ls -tr results_* | grep -v results_all | xargs -i% echo "\input {%}"
}

gen_plot_data()
{
    IFS=$'\n'
    for fsm in `ls -tr "$1"`; do
        echo -n "$fsm ";
    done
    echo

    i=1
    IFS=$' '
    for aspect in $TEST_ASPECTS; do

        if [ $2 -eq 0 ] || [ $i -eq $2 ]; then
            IFS=$'\n'
            for fsm in `ls -tr "$1"`; do
                if [ "$aspect" == "valgrind" ]; then
                    cat "$1/$fsm" | head -$i | tail -1 | sed "s/.*(\(.*\))/\1/" | tr -d ',' | egrep -o "[0-9\.]*" | xargs -i% echo -n "% "
                else
                    cat "$1/$fsm" | head -$i | tail -1 | egrep -o "[0-9\.]*"  | xargs -i% echo -n "% "
                fi
            done
            IFS=$' '
            echo
        fi

        ((i++))
    done
}

gen_relative_data()
{
    data=$1

    l=0
    while read line; do
        if [ $l -eq 0 ]; then
            echo $line
        else
            max=`echo "$line " | sed "s/\(.*\)\s/\1/" | tr ' ' '\n' | awk 'max=="" || $1 > max {max=$1} END {print max}'`

            IFS=$' '
            for value in $line; do
                if [ `echo "$max == 0" | bc` -eq 0 ]; then
                    echo "scale=2; (100.0 * $value / $max)" | bc | xargs -i% echo -n "% "
                else
                    echo -n "0.00 "
                fi
            done
            echo
            IFS=$'\n'
        fi
        ((l++))
    done < $data
}

gen_plot()
{
    gen_plot_data "$4" "$5" > data.dat

    if [ $9 -eq 1 ]; then
        cat data.dat > data.tmp
        gen_relative_data "data.tmp" > data.dat
        rm -f data.tmp
    fi

    xlabels=""
    if [ $5 -ne 0 ]; then
        xlabels="set xtics (\"\" 0)"
    fi

    FSMS_COUNT=`ls -tr "$4" | wc -l`
    plot="plot 'data.dat'"

    for ((c=1; c <= $FSMS_COUNT; ++c)); do
        if [ $c -eq 1 ]; then
            plot="`echo $plot` u $c ti col"
        else
            plot="`echo $plot`, '' u $c ti col"
        fi
    done

    if [ $WITH_TITLE -eq 1 ]; then
        gnuplot\
            -e "set terminal png transparent nocrop enhanced font arial 8 size $6"\
            -e "set style fill solid 1.00 border -1"\
            -e "set style data histograms"\
            -e "set output \"$1\""\
            -e "set key $7"\
            -e "set title\"$3\""\
            -e "$2" -e "$xlabels"\
            -e "$8"\
            -e "$plot"\
        2>/dev/null
    else
        gnuplot\
            -e "set terminal png transparent nocrop enhanced font arial 8 size $6"\
            -e "set style fill solid 1.00 border -1"\
            -e "set style data histograms"\
            -e "set output \"$1\""\
            -e "set key $7"\
            -e "$2" -e "$xlabels"\
            -e "$8"\
            -e "$plot"\
        2>/dev/null
    fi

    rm -f data.dat
}

gen_histogram()
{
    xtics="set xtics ("
    index=0
    IFS=$' '

    mkdir -p "`dirname \"$1\"`"

    for aspect in $TEST_ASPECTS; do
        [ $index -ne 0 ] && xtics="`echo $xtics`, "
        xtics="`echo $xtics` \"$aspect\" $index"

        ((index++))

        gen_plot "$1_$aspect.png" "set xlabel \"$aspect\"" "$3" "$2" "$index" "400,380" "above" "" 0
    done
    xtics="`echo $xtics`)"

    gen_plot "$1_all.png" "$xtics" "$3 [relative]" "$2" 0 "1024,600" "top left" "set yrange [0:150]" 1
}

gen_histograms()
{
    for comp in `ls -tr "$RESULTS"`; do
        for test in `ls -tr "$RESULTS/$comp"`; do
            gen_histogram "$DIR/images/results/$HOSTNAME/$comp/$test" "$RESULTS/$comp/$test" "$comp/$test"
        done
    done
}

gen_histograms
gen_tex > $DIR/results_$HOSTNAME.tex
gen_results $DIR > $DIR/results_all.tex

