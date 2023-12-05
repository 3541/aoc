#!/usr/bin/env bash

set -euo pipefail

INPUT="$1"

IFS=" " read -r -a seeds <<< "$(head -n1 "$INPUT" | cut -d':' -f2)"

min_location=$((2**30))
for seed in "${seeds[@]}"; do
    type="seed"
    item="$seed"

    while [[ "$type" != "location" ]]; do
        dest="$(grep -Po "${type}-to-\K[a-z]+" "$INPUT")"
        while read -r line; do
            dest_start="$(echo "$line" | cut -d' ' -f1)"
            source_start="$(echo "$line" | cut -d' ' -f2)"
            length="$(echo "$line" | cut -d' ' -f3)"

            if [[ "$item" -ge "$source_start" && "$item" -lt "$((source_start + length))" ]]; then
                item="$((dest_start + item - source_start))"
                break
            fi
        done < <(IFS="\n" grep -zoP "${type}-to-[a-z]+ map:\\n[0-9 \\n]+" "$INPUT" | tail -n +2 \
                     | head -n -2)
        type="$dest"

    done

    if [[ "$item" -lt "$min_location" ]]; then
        min_location="$item"
    fi
done

echo "$min_location"

IFS="," read -r -a ranges <<< "$(head -n1 "$INPUT" | sed 's/^[a-z]\+: //' | sed 's/\([0-9]\+ [0-9]\+\) /\1,/g')"

all=()
for range in "${ranges[@]}"; do
    start="$(echo "$range" | cut -d' ' -f1)"
    length="$(echo "$range" | cut -d' ' -f2)"
    type="seed"

    valid=("$range")

    while [[ "$type" != "location" ]]; do
        dest="$(grep -Po "${type}-to-\K[a-z]+" "$INPUT")"

        new_valid=()
        while read -r line; do
            dest_start="$(echo "$line" | cut -d' ' -f1)"
            source_start="$(echo "$line" | cut -d' ' -f2)"
            length="$(echo "$line" | cut -d' ' -f3)"

            for (( i=0; i<${#valid[@]}; )); do
                r="${valid[i]}"
                r_start="$(echo "$r" | cut -d' ' -f1)"
                r_length="$(echo "$r" | cut -d' ' -f2)"

                if [[ "$r_start" -ge "$source_start" && "$r_start" -lt "$((source_start + length))" ]]; then
                    unset "valid[$i]"
                    valid=("${valid[@]}")

                    len="$r_length"
                    if [[ "$len" -gt "$((length - (r_start - source_start)))" ]]; then
                        len="$((length - (r_start - source_start)))"
                    fi

                    new_valid+=("$((dest_start + r_start - source_start)) $len")

                    if [[ "$((r_start + r_length))" -gt "$((source_start + length))" ]]; then
                        valid+=("$((source_start + length)) $((r_length - (source_start + length - r_start)))")
                    fi
                elif [[ "$((r_start + r_length))" -gt "$source_start" && "$((r_start + r_length))" -le "$((source_start + length))" ]]; then
                    unset "valid[$i]"
                    valid=("${valid[@]}")

                    valid+=("$r_start $((source_start - r_start))")
                    new_valid+=("$dest_start $((r_length - (source_start - r_start)))")
                else
                    ((++i))
                fi
            done
        done < <(IFS="\n" grep -zoP "${type}-to-[a-z]+ map:\\n[0-9 \\n]+" "$INPUT" | tail -n +2 \
                     | head -n -2)

        valid+=("${new_valid[@]}")

        type="$dest"
    done

    all+=("${valid[@]}")
done

min=$((2**30))
for r in "${all[@]}"; do
    start="$(echo "$r" | cut -d' ' -f1)"

    if [[ "$start" -lt "$min" ]]; then
        min="$start"
    fi
done
echo "$min"
