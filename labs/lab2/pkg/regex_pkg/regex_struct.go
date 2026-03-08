package regex_pkg

import (
	"fmt"
	"strings"
)

type NodeType int

const (
	LeafNode NodeType = iota
	ConcatNode
	UnionNode
	KliniNode
	QuestionNode
	RepeatNode
	GroupNode
	RefNode
)

type Node struct {
	Type NodeType
	Value string
	Left *Node
	Right *Node
	Repeat int
	Nullable bool
	FirstPos []int
	LastPos []int
	Position int
}

type SyntaxTree struct{
	Root *Node
	Leaves []*Node
	PosCount int
}


func BuildSyntaxTree (regex string) (*SyntaxTree, error) {
	regex = "(" + regex + ")"
	tree := &SyntaxTree{
		Leaves:   make([]*Node, 0),
		PosCount: 0,
	}
	stack := make([]interface{}, 0)
	for i := 0; i < len(regex); i++ {
		ch := regex[i]

		switch {

		case ch == '(':
			fmt.Printf("открывающая (\n")
			if i+1 < len(regex) && regex[i+1] == '<' {
				// Ищем закрывающую '>' для имени группы
				endNamePos := -1
				var name string
				for j := i + 2; j < len(regex); j++ {
					if regex[j] == '<' {
						if j >= 1 && j+1 < len(regex) && regex[j-1] == '%' && regex[j+1] == '%' {
							// Это экранированный '>', пропускаем его
							regex = regex[:j-1] + string('_') + regex[j:]
							regex = regex[:j+1] + string('_') + regex[j+2:]
							name = name[:len(name)-1]
							name += string(regex[j])

							j += 1 // пропускаем символы '%' и '>'
							continue
						}
					}
					if regex[j] == '>' {
						if j >= 1 && j+1 < len(regex) && regex[j-1] == '%' && regex[j+1] == '%' {
							// Это экранированный '>', пропускаем его
							regex = regex[:j-1] + string('_') + regex[j:]
							regex = regex[:j+1] + string('_') + regex[j+2:]
							name = name[:len(name)-1]
							name += string(regex[j])

							j += 1 // пропускаем символы '%' и '>'
							continue
						}
						// Нашли НЕэкранированную '>'
						endNamePos = j
						break
					}
					name += string(regex[j])
				}
				fmt.Printf("%s\n", name)
				if endNamePos == -1 {
					return nil, fmt.Errorf("unclosed group name")
				}

				// Извлекаем имя группы (между '<' и '>')

				fmt.Printf("Имя группы: '%s'\n", name)

				// Кладем маркер группы в стек
				stack = append(stack, fmt.Sprintf("<%s>", name))

				// Перемещаем i на позицию '>'
				i = endNamePos
			} else {
				stack = append(stack, '(')
			}



		case ch == ')':
			fmt.Printf("закрывающая )\n")
			nodes := make([]interface{}, 0)
			var groupName string
			for len(stack) > 0 {
				top := stack[len(stack)-1]
				stack = stack[:len(stack)-1]
				if str, ok := top.(string); ok && strings.HasPrefix(str, "<") && strings.HasSuffix(str, ">") {
					// обработка маркера группы
					groupName = strings.TrimPrefix(str, "<")
					groupName = strings.TrimSuffix(groupName, ">")
					break
				}
				if top == '(' {
					break
				}
				nodes = append([]interface{}{top}, nodes...)
			}
			node := processNodes(nodes, tree) // тут мы типо превращаем выражение в () в один узел
			if groupName != "" {
				node = &Node{
					Type:  GroupNode,
					Value: groupName,
					Left:  node,
				}
			}
			stack = append(stack, node)
		case ch == '|':
			stack = append(stack, '|')
		case ch == '.' && i+2 < len(regex) && regex[i+1] == '.' && regex[i+2] == '.':
			if len(stack) > 0 {
				last := stack[len(stack)-1]
				if node, ok := last.(*Node); ok {
					starNode := &Node{
						Type: KliniNode,
						Left: node,
					}
					stack[len(stack)-1] = starNode
				}
			}
			i += 2
		case ch == '?':
			if len(stack) > 0 {
				last := stack[len(stack)-1]
				if node, ok := last.(*Node); ok {
					questNode := &Node{
						Type: QuestionNode,
						Left: node,
					}
					stack[len(stack)-1] = questNode
				}
			}
		case ch == '{':
			endBrace := strings.Index(regex[i:], "}")
			if endBrace == -1 {
				return nil, fmt.Errorf("unclosed figure brace")
			}
			var count int
			_, err := fmt.Sscanf(regex[i+1:i+endBrace], "%d", &count)
			if err != nil {
				return nil, err
			}
			if len(stack) > 0 {
				last := stack[len(stack)-1]
				if node, ok := last.(*Node); ok {
					repeatNode := &Node{
						Type:   RepeatNode,
						Left:   node,
						Repeat: count,
					}
					stack[len(stack)-1] = repeatNode
				}
			}
			i += endBrace
		case ch == '<' && i > 0 && regex[i-1] != '(':
			// Ищем закрывающую '>' для имени группы
			endNamePos := -1
			var name string
			for j := i + 1; j < len(regex); j++ {
				if regex[j] == '<' {
					if j >= 1 && j+1 < len(regex) && regex[j-1] == '%' && regex[j+1] == '%' {
						// Это экранированный '>', пропускаем его
						regex = regex[:j-1] + string('_') + regex[j:]
						regex = regex[:j+1] + string('_') + regex[j+2:]
						name = name[:len(name)-1]
						name += string(regex[j])

						j += 1 // пропускаем символы '%' и '>'
						continue
					}
				}
				if regex[j] == '>' {
					if j >= 1 && j+1 < len(regex) && regex[j-1] == '%' && regex[j+1] == '%' {
						// Это экранированный '>', пропускаем его
						regex = regex[:j-1] + string('_') + regex[j:]
						regex = regex[:j+1] + string('_') + regex[j+2:]
						name = name[:len(name)-1]
						name += string(regex[j])

						j += 1 // пропускаем символы '%' и '>'
						continue
					}
					// Нашли НЕэкранированную '>'
					endNamePos = j
					break
				}
				name += string(regex[j])
			}
			fmt.Printf("%s\n", name)
			if endNamePos == -1 {
				return nil, fmt.Errorf("unclosed group name")
			}

			refNode := &Node{
				Type:  RefNode,
				Value: name,
			}
			stack = append(stack, refNode)
			i = endNamePos

		default:
			fmt.Printf("Я в дефолте:  %s\n", ch)
			if ch == '%' && i+2 < len(regex) && i+2 < len(regex) && regex[i+2] == '%' {
				leafNode := createLeafNode(string(regex[i+1]), tree)
				stack = append(stack, leafNode)
				i += 2
			} else if !isMeta(ch) {
				leafNode := createLeafNode(string(ch), tree)
				stack = append(stack, leafNode)
			}
		}
	}
	if len(stack) != 1{
		return nil, fmt.Errorf("unknown regex")
	}
	tree.Root = stack[0].(*Node)
	return tree, nil
}



func createLeafNode (value string, tree *SyntaxTree) *Node {
	tree.PosCount++
	node := &Node{
		Type: LeafNode,
		Value: value,
		Position: tree.PosCount,
	}
	tree.Leaves = append(tree.Leaves, node)
	return node;
}

func createEpsilonNode() *Node{
	node := &Node{
		Type: LeafNode,
		Value: "epsilon",
		Position: 0,
	}
	return node
}

func processNodes(nodes []interface{}, tree *SyntaxTree) *Node {
	if len(nodes) == 0{
		return createEpsilonNode()
	}
	var result *Node
	current := make([]interface{}, 0)
	for i := 0; i < len(nodes); i++ {
		if nodes[i] == '|' {
			left := processConcat(current, tree)
			right := processNodes(nodes[i+1:], tree)

			result = &Node{
				Type:  UnionNode,
				Left:  left,
				Right: right,
			}
			break
		} else {
			current = append(current, nodes[i])
		}
	}
	if result == nil{
		result = processConcat(current, tree)
	}
	return result
}

func processConcat(nodes []interface{}, tree *SyntaxTree) *Node {
	if len(nodes) == 0{
		return createEpsilonNode()
	}
	if len(nodes) == 1{
		return nodes[0].(*Node)
	}
	//тут построение дерева
	var result *Node
	for i := 0; i < len(nodes); i++ {
		if node, ok := nodes[i].(*Node); ok{
			if result == nil{
				result = node
			} else {
				result = &Node{
					Type: ConcatNode,
					Left: result,
					Right: node,
				}
			}
		}
	}
	return result
}

func isMeta(ch byte) bool {
	return ch == '(' || ch == ')' || ch == '|' || ch == '.' ||
		ch == '?' || ch == '{' || ch == '}' || ch == '<' ||
		ch == '>' || ch == '%'
}


func (tree *SyntaxTree) Print() {
	fmt.Println("=== Syntax Tree ===")
	printNode(tree.Root, 0)
	fmt.Println("Leaves:")
	for _, leaf := range tree.Leaves {
		fmt.Printf("  Pos %d: %s\n", leaf.Position, leaf.Value)
	}
}


// для отладки, потом уберу
func printNode(node *Node, level int) {
	if node == nil {
		return
	}

	indent := strings.Repeat("  ", level)

	switch node.Type {
	case LeafNode:
		fmt.Printf("%sLeaf: %s (pos:%d)\n", indent, node.Value, node.Position)
	case ConcatNode:
		fmt.Printf("%sConcat\n", indent)
		printNode(node.Left, level+1)
		printNode(node.Right, level+1)
	case UnionNode:
		fmt.Printf("%sUnion\n", indent)
		printNode(node.Left, level+1)
		printNode(node.Right, level+1)
	case KliniNode:
		fmt.Printf("%sKlini (...)\n", indent)
		printNode(node.Left, level+1)
	case QuestionNode:
		fmt.Printf("%sQuestion (?)\n", indent)
		printNode(node.Left, level+1)
	case RepeatNode:
		fmt.Printf("%sRepeat{%d}\n", indent, node.Repeat)
		printNode(node.Left, level+1)
	case GroupNode:
		fmt.Printf("%sGroup(%s)\n", indent, node.Value)
		printNode(node.Left, level+1)
	case RefNode:
		fmt.Printf("%sRef(%s)\n", indent, node.Value)
	}
}