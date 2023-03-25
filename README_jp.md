# BindingEase

Blueprintでのイベントバインディング手法が幾つか存在していますが、視認性が悪かったり  
使い勝手が悪い為自作したノードで視認性と機能を拡張した物を用意しました。

以下はUnrealEngineのBlueprintで提供されているイベントバインディングの手法です。

### 通常のイベントバインディング

![Standard Blueprint Binding Methods](https://media.githubusercontent.com/media/laycnc/UnrealBindingEase/image/Doc/Image/StandardBlueprintBindingMethods.png "Standard Blueprint Binding Methods")

Blueprintで標準的なイベントバインディングを行うと必ずイベントノードを繋げる必要があります。  

* メリット    
バインディングしているイベントに引数を変更した場合に自動では変更が適応される。  
イベントと実装への紐づけが一目で分かる。  
* デメリット  
イベントの実装量が増えると視認性が非常に悪くなる。  

### CreateEventノードを使ったイベントバインディング

![Standard Blueprint Binding Methods](https://media.githubusercontent.com/media/laycnc/UnrealBindingEase/image/Doc/Image/CreateEventNodeMethod.png "Standard Blueprint Binding Methods")

CreateNodeとカスタムイベントノードを紐づける事で行こなえる手法です。  

* メリット  
実装箇所と配置場所を分ける事が出来ます。  
* デメリット  
バインディング箇所と実装箇所が分かれている為、バインディングされているかが分かり難い点  
バインディングしているイベントに引数を変更した場合に自動では変更が適応されない。  

### プロパティへのDynamicBindingを使ったイベントバインディング

![Standard Blueprint Binding Methods](https://media.githubusercontent.com/media/laycnc/UnrealBindingEase/image/Doc/Image/DynamicBindingMethod.png "Standard Blueprint Binding Methods")

変数の詳細タブのイベントカテゴリに存在するボタンを押す事でバインディングノードを作成する事ができる手法です。  

* メリット  
イベントノードの配置場所に自由度が高い為、視認性に優れています。  
バインディングしているイベントに引数を変更した場合に自動では変更が適応される。  
* デメリット  
ActorやWidgetなどの一部のクラスでしか扱う事が出来ない点  
Spawn時に公開された変数など、初期化タイミングによっては扱う事が出来ない点  
一度バインディングを行うと、アンバインドが手動では行えない点  



## 自作したイベントバインディング手法に関して

![Standard Blueprint Binding Methods](https://media.githubusercontent.com/media/laycnc/UnrealBindingEase/image/Doc/Image/BindingEaseMethod.png "Standard Blueprint Binding Methods")




# Binding Easeノードのイベントへの実装方法  

![Standard Blueprint Binding Methods](https://raw.githubusercontent.com/laycnc/UnrealBindingEase/image/Doc/Image/BindingEase.gif "Standard Blueprint Binding Methods")

コンテキストメニューから*Binding Ease*を選択してノードを作成。  
ノードを選択して、バインドしたいイベントを所持しているクラスを選択する。  
クラスを選択すると*DelegatePropertyName*を選択できるようになります。  
バインドしたいイベントをプルダウンから選ぶと、Delegateと同じシグネチャにノードが変化します。  
あとは必要な機能を実装してください。  