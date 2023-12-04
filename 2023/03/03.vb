Imports System

Module Program
    Sub Main(args As String())
       Dim Input() As String = Split(System.IO.File.ReadAllText(args(0)), Environment.NewLine)

       Dim grid(Input.Length - 2)() As Char
       Dim ratios(Input.Length - 2)() As Integer
       Dim counts(Input.Length - 2)() As Integer
       For r = 0 To grid.Length - 1
          grid(r) = New Char(Len(Input(r)) - 1) {}
          ratios(r) = New Integer(Len(Input(r)) - 1) {}
          counts(r) = New Integer(Len(Input(r)) - 1) {}
          For c = 0 To grid(r).Length - 1
             grid(r)(c) = Input(r).Chars(c)
             ratios(r)(c) = 1
             counts(r)(c) = 0
          Next
       Next

       Dim total As Integer = 0
       For i As Integer = 0 To grid.Length - 1
          Dim j As Integer = 0
          While j < grid(i).Length
             If IsNumeric(grid(i)(j)) Then
                Dim start As Integer = j
                Dim finish As Integer = start
                Dim part As String = ""
                Dim part_i As Integer = 0
                Dim part_j As Integer = 0
                While finish < grid(i).Length AndAlso IsNumeric(grid(i)(finish))
                   For i2 As Integer = i - 1 To i + 1
                      If i2 < 0 Or i2 >= grid.getUpperBound(0) Then
                         Continue For
                      End If

                      For j2 As Integer = finish - 1 To finish + 1
                         If j2 < 0 Or j2 >= grid(i).Length Then
                            Continue For
                         End If

                         If Not IsNumeric(grid(i2)(j2)) And grid(i2)(j2) <> "." And part <> "*" Then
                            part = grid(i2)(j2)
                            part_i = i2
                            part_j = j2
                         End If
                      Next
                   Next

                   finish += 1
                End While

                If start <> finish Then
                   j = finish

                   If part <> "" Then
                      Dim n(finish - start - 1) As Char
                      Array.copy(grid(i), start, n, 0, finish - start)

                      Dim s As String = New String(n)
                      Dim v As Integer = Val(s)
                      total += v

                      If part = "*" Then
                         ratios(part_i)(part_j) *= v
                         counts(part_i)(part_j) += 1
                      End If
                   End If
                End If
             End If
             j += 1
          End While
       Next

       Console.WriteLine(total)

       Dim ratio As Integer = 0
       For i As Integer = 0 To ratios.Length - 1
          For j As Integer = 0 To ratios(i).Length - 1
             If counts(i)(j) <> 2 Then
                Continue For
             End If

             ratio += ratios(i)(j)
          Next
       Next

       Console.WriteLine(ratio)
    End Sub
End Module
