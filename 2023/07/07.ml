module Label = struct
  type t =
    | Joker
    | Two
    | Three
    | Four
    | Five
    | Six
    | Seven
    | Eight
    | Nine
    | Ten
    | Jack
    | Queen
    | King
    | Ace

  let all =
    [
      Two;
      Three;
      Four;
      Five;
      Six;
      Seven;
      Eight;
      Nine;
      Ten;
      Jack;
      Queen;
      King;
      Ace;
      Joker;
    ]

  let all_regular = List.filter (fun l -> l <> Joker) all
  let compare = compare
end

module LabelMap = Map.Make (Label)

module Rank = struct
  type t =
    | HighCard
    | Pair
    | TwoPair
    | ThreeKind
    | FullHouse
    | FourKind
    | FiveKind
end

module Hand = struct
  type t = Label.t list

  let rank (h : t) : Rank.t =
    let counts : int LabelMap.t =
      List.fold_left
        (fun (m : int LabelMap.t) (l : Label.t) ->
          LabelMap.update l
            (fun (v : int Option.t) ->
              match v with
              | None -> Option.some 1
              | Option.Some c -> Option.some (c + 1))
            m)
        LabelMap.empty h
    in
    let unique : int = LabelMap.cardinal counts in
    let max_count : int =
      LabelMap.to_list counts |> List.map snd |> List.sort compare |> List.rev
      |> List.hd
    in
    match max_count with
    | 5 -> Rank.FiveKind
    | 4 -> Rank.FourKind
    | 3 when unique = 2 -> Rank.FullHouse
    | 3 when unique = 3 -> Rank.ThreeKind
    | 2 when unique = 3 -> Rank.TwoPair
    | 2 when unique = 4 -> Rank.Pair
    | _ -> Rank.HighCard

  let rank2 (h : t) : Rank.t =
    if List.exists (( = ) Label.Joker) h then
      List.map
        (fun r ->
          List.map (fun l -> if l = Label.Joker then r else l) h |> rank)
        Label.all_regular
      |> List.sort compare |> List.rev |> List.hd
    else rank h

  let compare (rank : t -> Rank.t) (l : t) (r : t) =
    match compare (rank l) (rank r) with
    | 0 -> List.compare Label.compare l r
    | n -> n

  let parse (p2 : bool) (s : String.t) : t =
    String.fold_left
      (fun (l : t) (c : char) ->
        (match c with
        | '2' -> Label.Two
        | '3' -> Label.Three
        | '4' -> Label.Four
        | '5' -> Label.Five
        | '6' -> Label.Six
        | '7' -> Label.Seven
        | '8' -> Label.Eight
        | '9' -> Label.Nine
        | 'T' -> Label.Ten
        | 'J' -> if p2 then Label.Joker else Label.Jack
        | 'Q' -> Label.Queen
        | 'K' -> Label.King
        | 'A' -> Label.Ace
        | _ -> failwith "Invalid card")
        :: l)
      [] s
    |> List.rev
end

let winnings (rank : Hand.t -> Rank.t) (h : (Hand.t * int) list) =
  List.sort (fun (l, _) (r, _) -> Hand.compare rank l r) h
  |> List.mapi (fun i (_, w) -> (i + 1) * w)
  |> List.fold_left ( + ) 0

let input =
  let file = open_in_bin Sys.argv.(1) in
  let s = really_input_string file (in_channel_length file) in
  close_in file;
  s

let result ~(p2 : bool) =
  String.split_on_char '\n' input
  |> List.filter (fun s -> String.length s > 0)
  |> List.map (fun s ->
         let a = String.split_on_char ' ' s in
         (List.hd a |> Hand.parse p2, List.nth a 1 |> int_of_string))
  |> winnings (if p2 then Hand.rank2 else Hand.rank)
;;

result ~p2:false |> string_of_int |> print_endline;;
result ~p2:true |> string_of_int |> print_endline
