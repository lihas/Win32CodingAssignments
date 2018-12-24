' for 'Form'
Imports System.Windows.Forms
' .Net callable dll created by using tlbimp.exe utility
Imports AutomationServerTypeLibForDotNet

Public Class VBAutomationClient
    Inherits Form
    Public Sub New()
        Dim MyIDispatch As Object
        Dim MyRef As New CMyMathClass
        MyIDispatch = MyRef
        Dim iNum1 = 175
        Dim iNum2 = 125
        Dim iSum = MyIDispatch.SumOfTwoIntegers(iNum1, iNum2)
        Dim str As String = String.Format("Sum of {0} and {1} is {2}", iNum1, iNum2, iSum)
        ' Default messageb box with only 1 button - OK
        MsgBox(str)
        Dim iSub = MyIDispatch.SubtractionOfTwoIntegers(iNum1, iNum2)
        str = String.Format("Subtraction of {0} and {1} is {2}", iNum1, iNum2, iSub)
        MsgBox(str)
        ' Following statement, i.e. 'End' works as DestroyWindow(hwnd)
        End
    End Sub

    <STAThread()>
    Shared Sub Main()
        Application.EnableVisualStyles()
        Application.Run(New VBAutomationClient())
    End Sub
End Class