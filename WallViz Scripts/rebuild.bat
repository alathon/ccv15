del /s /f /q C:\Users\wallvizpc\Desktop\ccvRun\*.*
for /f %%f in ('dir /ad /b C:\Users\wallvizpc\Desktop\ccvRun\') do rd /s /q C:\Users\wallvizpc\Desktop\ccvRun\%%f

xcopy "C:\Users\wallvizpc\workspace\ccv15\apps\addonsExamples\VS2010\bin" "C:\Users\wallvizpc\Desktop\ccvRun\1\" /S /Y
xcopy "C:\Users\wallvizpc\workspace\ccv15\apps\addonsExamples\VS2010\bin" "C:\Users\wallvizpc\Desktop\ccvRun\2\" /S /Y
xcopy "C:\Users\wallvizpc\workspace\ccv15\apps\addonsExamples\VS2010\bin" "C:\Users\wallvizpc\Desktop\ccvRun\3\" /S /Y

xcopy "C:\Users\wallvizpc\workspace\ccv15\WallViz Scripts\xml1" "C:\Users\wallvizpc\Desktop\ccvRun\1\data\xml\" /S /Y
xcopy "C:\Users\wallvizpc\workspace\ccv15\WallViz Scripts\xml2" "C:\Users\wallvizpc\Desktop\ccvRun\2\data\xml\" /S /Y
xcopy "C:\Users\wallvizpc\workspace\ccv15\WallViz Scripts\xml3" "C:\Users\wallvizpc\Desktop\ccvRun\3\data\xml\" /S /Y

@echo start "C:\Users\wallvizpc\Desktop\ccvRun\1\CCV.exe" >> "C:\Users\wallvizpc\Desktop\ccvRun\run.bat"
@echo start "C:\Users\wallvizpc\Desktop\ccvRun\2\CCV.exe" >> "C:\Users\wallvizpc\Desktop\ccvRun\run.bat"
@echo start "C:\Users\wallvizpc\Desktop\ccvRun\3\CCV.exe" >> "C:\Users\wallvizpc\Desktop\ccvRun\run.bat"