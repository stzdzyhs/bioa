object Form10: TForm10
  Left = 0
  Top = 0
  Caption = #29983#21270#20998#26512#20202
  ClientHeight = 648
  ClientWidth = 931
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 931
    Height = 85
    ButtonHeight = 84
    ButtonWidth = 71
    Caption = 'ToolBar1'
    Flat = False
    Images = ImageList1
    ShowCaptions = True
    TabOrder = 0
    object ToolButton1: TToolButton
      Left = 0
      Top = 0
      Caption = #26816#27979
      ImageIndex = 0
      OnClick = ToolButton1Click
    end
    object ToolButton2: TToolButton
      Left = 71
      Top = 0
      Caption = #35797#21058
      ImageIndex = 1
      OnClick = ToolButton2Click
    end
    object ToolButton3: TToolButton
      Left = 142
      Top = 0
      Caption = #23450#26631
      ImageIndex = 2
      OnClick = ToolButton3Click
    end
    object ToolButton4: TToolButton
      Left = 213
      Top = 0
      Caption = #36136#25511
      ImageIndex = 3
      OnClick = ToolButton4Click
    end
    object ToolButton5: TToolButton
      Left = 284
      Top = 0
      Caption = #32500#25252#20445#20859
      ImageIndex = 4
      OnClick = ToolButton5Click
    end
    object ToolButton6: TToolButton
      Left = 355
      Top = 0
      Caption = #31995#32479#35774#32622
      ImageIndex = 5
      OnClick = ToolButton6Click
    end
  end
  object Panel1: TPanel
    Left = 820
    Top = 85
    Width = 111
    Height = 544
    Align = alRight
    BevelOuter = bvNone
    BorderWidth = 5
    Color = clBackground
    ParentBackground = False
    TabOrder = 1
    object BitBtn1: TBitBtn
      Left = 5
      Top = 425
      Width = 101
      Height = 84
      Align = alTop
      Caption = 'BitBtn1'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      TabOrder = 0
    end
    object BitBtn2: TBitBtn
      Left = 5
      Top = 341
      Width = 101
      Height = 84
      Align = alTop
      Caption = 'BitBtn1'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      TabOrder = 1
    end
    object BitBtn3: TBitBtn
      Left = 5
      Top = 257
      Width = 101
      Height = 84
      Align = alTop
      Caption = 'BitBtn1'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      TabOrder = 2
    end
    object BitBtn4: TBitBtn
      Left = 5
      Top = 173
      Width = 101
      Height = 84
      Align = alTop
      Caption = 'BitBtn1'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      TabOrder = 3
    end
    object BitBtn5: TBitBtn
      Left = 5
      Top = 89
      Width = 101
      Height = 84
      Align = alTop
      Caption = 'BitBtn1'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      TabOrder = 4
    end
    object BitBtn6: TBitBtn
      Left = 5
      Top = 5
      Width = 101
      Height = 84
      Align = alTop
      Caption = 'BitBtn1'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      TabOrder = 5
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 629
    Width = 931
    Height = 19
    Panels = <
      item
        Text = #25805#20316#21592':admin'
        Width = 150
      end
      item
        Width = 50
      end>
  end
  object pcWorkArea: TPageControl
    Left = 0
    Top = 85
    Width = 820
    Height = 544
    ActivePage = TabSheet4
    Align = alClient
    Style = tsFlatButtons
    TabOrder = 3
    object TabSheet4: TTabSheet
      Caption = 'TabSheet4'
      object PageControl1: TPageControl
        Left = 0
        Top = 0
        Width = 812
        Height = 513
        ActivePage = TabSheet1
        Align = alClient
        TabOrder = 0
        object TabSheet1: TTabSheet
          Caption = #26679#26412#30331#35760
          object Label1: TLabel
            Left = 184
            Top = 11
            Width = 24
            Height = 13
            Caption = #20301#32622
          end
          object Label2: TLabel
            Left = 320
            Top = 11
            Width = 48
            Height = 13
            Caption = #26679#26412#31867#22411
          end
          object LabeledEdit1: TLabeledEdit
            Left = 32
            Top = 30
            Width = 121
            Height = 21
            EditLabel.Width = 36
            EditLabel.Height = 13
            EditLabel.Caption = #26679#26412#21495
            TabOrder = 0
          end
          object ComboBox1: TComboBox
            Left = 184
            Top = 30
            Width = 89
            Height = 21
            TabOrder = 1
            Items.Strings = (
              '1'
              '2'
              '3'
              '4'
              '5'
              '6'
              '7'
              '8'
              '9'
              '10'
              '11'
              '12'
              '13'
              '....')
          end
          object ComboBox2: TComboBox
            Left = 320
            Top = 30
            Width = 89
            Height = 21
            TabOrder = 2
            Items.Strings = (
              '1'
              '2'
              '3'
              '4'
              '5'
              '6'
              '7'
              '8'
              '9'
              '10'
              '11'
              '12'
              '13'
              '....')
          end
        end
        object TabSheet2: TTabSheet
          Caption = #26679#26412#32467#26524
          ImageIndex = 1
        end
        object TabSheet3: TTabSheet
          Caption = #27979#35797#32467#26524
          ImageIndex = 2
        end
      end
    end
    object TabSheet5: TTabSheet
      Caption = 'TabSheet5'
      ImageIndex = 1
      object Label3: TLabel
        Left = 88
        Top = 40
        Width = 63
        Height = 13
        Caption = 'this is page 2'
      end
    end
    object TabSheet6: TTabSheet
      Caption = 'TabSheet6'
      ImageIndex = 2
      object Label4: TLabel
        Left = 88
        Top = 40
        Width = 63
        Height = 13
        Caption = 'this is page 3'
      end
    end
    object TabSheet7: TTabSheet
      Caption = 'TabSheet7'
      ImageIndex = 3
    end
    object TabSheet8: TTabSheet
      Caption = 'TabSheet8'
      ImageIndex = 4
    end
    object TabSheet9: TTabSheet
      Caption = 'TabSheet9'
      ImageIndex = 5
    end
  end
  object ImageList1: TImageList
    Height = 64
    Width = 64
    Left = 432
    Top = 296
  end
end
