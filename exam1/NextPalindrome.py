def NextPalindrome(palindrome):
    if int(palindrome[2])<9:
        newNum = int(palindrome[2])+1
        newPalindrome = palindrome[:2]+str(newNum)+palindrome[3:]
        return newPalindrome
    elif int(palindrome[2]) == 9 and int(palindrome[1]) != 9:
        newNum1 = int(palindrome[1])+1
        newNum2 = 0
        newNum3 = int(palindrome[3])+1
        newPalindrome = palindrome[0]+str(newNum1)+str(newNum2)+str(newNum3)+palindrome[4]
        return newPalindrome
    else:
        newNum1 = int(palindrome[0])+1
        newNum2 = 0
        newNum3 = 0
        newNum4 = 0
        newNum5 = int(palindrome[4])+1
        newPalindrome = str(newNum1)+str(newNum2)+str(newNum3)+str(newNum4)+str(newNum5)
        return newPalindrome
        
print(NextPalindrome('49994'))




