# BindingEase

Blueprintでのイベントバインディング手法が幾つか存在していますが、視認性が悪かったり  
使い勝手が悪い為自作したノードで視認性と機能を拡張した物を用意しました。

以下はUnrealEngineのBlueprintで提供されているイベントバインディングの手法です。

### 通常のイベントバインディング

Blueprintで標準的なイベントバインディングを行うと必ずイベントノードを繋げる必要があります。  
その為、ノードの配置場所に自由度が減り、視認性が悪くなります。

![Standard Blueprint Binding Methods](https://media.githubusercontent.com/media/laycnc/UnrealBindingEase/image/Doc/Image/StandardBlueprintBindingMethods.png "Standard Blueprint Binding Methods")

### CreateEventノードを使ったイベントバインディング

CreateEventノードを使う事でバインディング箇所と実際のイベントの実装箇所を分離する事が出来ます。  
実際の実装箇所であるカスタムノードの配置場所への自由度が増えますが、ノードの数が増える為やはり視認性が悪いように感じます。  

![Standard Blueprint Binding Methods](https://media.githubusercontent.com/media/laycnc/UnrealBindingEase/image/Doc/Image/CreateEventNodeMethod.png "Standard Blueprint Binding Methods")

### プロパティへのDynamicBindingを使ったイベントバインディング

視認性的には非常に理想的に思えます。  
しかしながら、ActorやWidgetなど一部の特殊なクラスでしか扱うことが出来ません。  
一度イベントバインディングされると、アンバインドを行う事が出来ないのもデメリットです。

![Standard Blueprint Binding Methods](https://media.githubusercontent.com/media/laycnc/UnrealBindingEase/image/Doc/Image/DynamicBindingMethod.png "Standard Blueprint Binding Methods")


## 自作したイベントバインディング手法に関して

![Standard Blueprint Binding Methods](https://media.githubusercontent.com/media/laycnc/UnrealBindingEase/image/Doc/Image/BindingEaseMethod.png "Standard Blueprint Binding Methods")



![Standard Blueprint Binding Methods](https://raw.githubusercontent.com/laycnc/UnrealBindingEase/image/Doc/Image/BindingEase.gif "Standard Blueprint Binding Methods")

Binding method using CreateEventNode in Blueprint

