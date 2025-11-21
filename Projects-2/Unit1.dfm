object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 441
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  OnShow = FormShow
  TextHeight = 15
  object ComboBox1: TComboBox
    Left = 40
    Top = 72
    Width = 513
    Height = 23
    TabOrder = 0
    OnChange = ComboBox1Change
  end
  object Button1: TButton
    Left = 40
    Top = 248
    Width = 513
    Height = 25
    Caption = 'set the enter with current usb-key'
    TabOrder = 1
    OnClick = Button1Click
  end
  object StaticText1: TStaticText
    Left = 48
    Top = 112
    Width = 41
    Height = 19
    Caption = 'Model:'
    TabOrder = 2
  end
  object StaticText2: TStaticText
    Left = 48
    Top = 145
    Width = 32
    Height = 19
    Caption = 'Type:'
    TabOrder = 3
  end
  object StaticText3: TStaticText
    Left = 48
    Top = 178
    Width = 35
    Height = 19
    Caption = 'Serial:'
    TabOrder = 4
  end
  object StaticText4: TStaticText
    Left = 48
    Top = 211
    Width = 27
    Height = 19
    Caption = 'Size:'
    TabOrder = 5
  end
  object StaticText5: TStaticText
    Left = 120
    Top = 112
    Width = 34
    Height = 19
    Caption = 'DATA'
    TabOrder = 6
  end
  object StaticText6: TStaticText
    Left = 120
    Top = 145
    Width = 34
    Height = 19
    Caption = 'DATA'
    TabOrder = 7
  end
  object StaticText7: TStaticText
    Left = 120
    Top = 178
    Width = 34
    Height = 19
    Caption = 'DATA'
    TabOrder = 8
  end
  object StaticText8: TStaticText
    Left = 120
    Top = 211
    Width = 34
    Height = 19
    Caption = 'DATA'
    TabOrder = 9
  end
  object SaveDialog1: TSaveDialog
    Left = 568
    Top = 248
  end
  object OpenDialog1: TOpenDialog
    Left = 568
    Top = 72
  end
end
