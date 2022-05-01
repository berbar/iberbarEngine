

declare namespace iberbar.Xml
{
    class CDocument
    {
        protected constructor();
        public ReadFile( filepath: string ): string | true;
        public ReadString( source: string ): string | true;
        public GetNodeRoot(): CNode;
    }

    class CNode
    {
        protected constructor();
        public SelectSingleNode( name: string ): CNode;
        public SelectNodes( name: string ): CNode[];
        public GetAttribute( name: string ): string;
        public GetValueText(): string;
        public GetNameText(): string;
    }

    function CreateRapidDocument( this: void ): CDocument;
}