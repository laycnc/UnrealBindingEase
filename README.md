# BindingEase

There are several methods of event binding in Blueprint, but they are not very visible or easy to use.  
I have prepared a node that I created myself to extend visibility and functionality.

The following is the event binding method provided by UnrealEngine's Blueprint.

### Normal Event Binding

![Standard Blueprint Binding Methods](https://media.githubusercontent.com/media/laycnc/UnrealBindingEase/image/Doc/Image/StandardBlueprintBindingMethods.png "Standard Blueprint Binding Methods")

Standard event binding in Blueprint always requires event nodes to be connected.  

* Advantages    
If you change the arguments of a binding event, the change will be applied automatically.  
The binding of events to implementations can be seen at a glance.  
* Disadvantages  
Visibility becomes very poor when the amount of event implementation increases.  

### Event binding using CreateEvent node

![Standard Blueprint Binding Methods](https://media.githubusercontent.com/media/laycnc/UnrealBindingEase/image/Doc/Image/CreateEventNodeMethod.png "Standard Blueprint Binding Methods")

This method can be performed by binding a CreateNode to a custom event node.  

* Advantages  
Separate implementation and placement locations are possible.  
* Disadvantages  
Since the binding location and the implementation location are separated, it is difficult to know if the binding is done or not.  
When arguments are changed for a bound event, the change is not automatically applied.  

### Event binding using DynamicBinding to properties

![Standard Blueprint Binding Methods](https://media.githubusercontent.com/media/laycnc/UnrealBindingEase/image/Doc/Image/DynamicBindingMethod.png "Standard Blueprint Binding Methods")

This method allows you to create a binding node by pressing a button that exists in the event category of the variable's details tab.  

* Advantages  
High visibility due to the high degree of freedom in the placement of event nodes.  
When an argument is changed in a binding event, the change is automatically applied.  
* Disadvantages  
Can only be handled by some classes such as Actor and Widget.  
Cannot handle variables exposed at spawn time, depending on initialization timing.  
Once binding is done, unbinding cannot be done manually.  



## Regarding the event binding method I created.

![Standard Blueprint Binding Methods](https://media.githubusercontent.com/media/laycnc/UnrealBindingEase/image/Doc/Image/BindingEaseMethod.png "Standard Blueprint Binding Methods")


## How to implement Binding Ease node to events  

![Standard Blueprint Binding Methods](https://raw.githubusercontent.com/laycnc/UnrealBindingEase/image/Doc/Image/BindingEase.gif "Standard Blueprint Binding Methods")

Create a node by selecting *Binding Ease* from the context menu.  
Select the node and choose the class that has the event you wish to bind.  
After selecting the class, you will be able to select *DelegatePropertyName*.  
Select the event you want to bind from the pull-down and the node will change to the same signature as the Delegate.  
Now you can implement the functionality you need.  

![Standard Blueprint Binding Methods](https://media.githubusercontent.com/media/laycnc/UnrealBindingEase/image/Doc/Image/BindingEaseRegisterAndUnregister.png "Standard Blueprint Binding Methods")

The actual binding and unbinding of events is done in a batch of functions.  
Binding and unbinding can be performed by passing an object of the class specified in the created node as an argument.

Translated with www.DeepL.com/Translator (free version)