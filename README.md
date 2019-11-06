# lw-example-shuffle-n-select-from-file
Example code to shuffle and select sentences from file

# 목표
여러줄의 문장을 랜덤한 숫자만큼 선택하여 새로운 파일에 기록한다

# 방법
- 문장 시작 위치 : 한줄씩 읽어가며 ftell로 position 기록
- 문장 랜덤 선택 : 기록한 position 위치를 shuffing
- 문장 랜덤 기록 : shuffling 한 위치를 fseek로 찾아가면서 문장을 읽어서 기록
