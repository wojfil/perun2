import subprocess

def runTestCase(code, expectedResult):
  cmd = "uro -d res -c \"" + code + "\""
  output = subprocess.getoutput(cmd)
  if output != expectedResult:
    print("Test failed at running code: " + code)
    print("  Expected output: " + expectedResult)
    print("  Received output: " + output)
    
def lines(*args):
  return "\n".join(args)
  
  
print ("BLACK-BOX TESTS START")
	
runTestCase("print 'hello world'", "hello world")
runTestCase("print 2 +2*2", "6")
runTestCase("print 2, 5", lines("2", "5"))
runTestCase("print 2, 2*25, 1", lines("2", "50", "1"))
runTestCase("if true {43} else {21}", "43")
runTestCase("if true and false {43} else {21}", "21")
runTestCase(" 'a.txt' {  exists; size }  ", lines("1", "47"))
	
print ("BLACK-BOX TESTS END")
print ("All tests have passed successfully if there is no error message above.")
input("Press Enter to continue...")