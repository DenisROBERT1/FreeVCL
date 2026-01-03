object Form_Principale: TForm_Principale
  Left = 100
  Top = 50
  Width = 300
  Height = 200
  Position = poScreenCenter
  Caption = 'Unité {{{FILE_NAME}}}'
  object Button_Ok: TButton
    Left = 110
    Top = 120
    Width = 80
    Height = 23
    Anchors = [akLeft, akBottom]
    Caption = 'Ok'
    OnClick = Button_OkClick
    Default = False
  end
end
